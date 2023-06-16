#include <iostream>
#include <bitset>
#include <cstring>

#include <intrin.h>
#include <zlib.h>

#include "png.h"

Png::Png(std::string puth)
{
	std::ifstream image;
	image.open(puth, std::ios::in | std::ios::binary | std::ios::ate);
	int length = image.tellg();
	image.seekg(0);

	char header[8];
	image.read(header, 8);
	if (header[0] == '\x89' && header[1] == 'P' && header[2] == 'N' && header[3] == 'G'
		&& header[4] == '\r' && header[5] == '\n' && header[6] == '\x1a' && header[7] == '\n')
	{
		int i = 8;
		while (i < length)
		{
			readChunk(&image, i);
		}
	}
	else
	{
		throw(WrongFileFormaat);
	}
}

void Png::readChunk(std::ifstream* image, int& i) noexcept
{
	image->seekg(i);

	Chunk temp_chunk;
	image->read(reinterpret_cast<char*>(&temp_chunk.length), 4);
	temp_chunk.length = _byteswap_ulong(temp_chunk.length);

	updatePosRead(image, i, i + 4);
	image->read(reinterpret_cast<char*>(&temp_chunk.chunk_name), 4);


	updatePosRead(image, i, i + 4);
	for (int j = 0; j < temp_chunk.length; j++)
	{
		temp_chunk.chunk_data.emplace_back();
	}

	image->read(reinterpret_cast<char*>(temp_chunk.chunk_data.data()), temp_chunk.length);
	updatePosRead(image, i, i+temp_chunk.length);

	image->read(reinterpret_cast<char*>(&temp_chunk.crc), 4);
	i += 4;

	switch (temp_chunk.chunk_name)
	{
	case 1380206665:
		width = static_cast<uint32_t>(temp_chunk.chunk_data[0]) << 24 | static_cast<uint32_t>(temp_chunk.chunk_data[1]) << 16 | static_cast<uint32_t>(temp_chunk.chunk_data[2]) << 8 | static_cast<uint32_t>(temp_chunk.chunk_data[3]);
		height = static_cast<uint32_t>(temp_chunk.chunk_data[4]) << 24 | static_cast<uint32_t>(temp_chunk.chunk_data[5]) << 16 | static_cast<uint32_t>(temp_chunk.chunk_data[6]) << 8 | static_cast<uint32_t>(temp_chunk.chunk_data[7]);
		bit_depth = static_cast<uint8_t>(temp_chunk.chunk_data[8]);
		color_type = static_cast<Png::ColorType>(temp_chunk.chunk_data[9]);

		if (bit_depth != 1 && bit_depth != 2 && bit_depth != 4 && bit_depth != 8 && bit_depth != 16)
		{
			throw InvalidChunkDataValue;
		}
		break;
	case 1163152464:
	{
		int k = 0;
		for (int j = 0; j < temp_chunk.chunk_data.size(); j += 3)
		{
			data.emplace_back();
			data[k].red = static_cast<uint8_t>(temp_chunk.chunk_data[j]);
			data[k].green = static_cast<uint8_t>(temp_chunk.chunk_data[j + 1]);
			data[k].blue = static_cast<uint8_t>(temp_chunk.chunk_data[j + 2]);
			k++;
		}
		break;
	}
	case 1413563465:
		switch (color_type)
		{
		case Png::greyscale:
			return;
			break;
		case Png::truecolor:
		{
			auto decompress_data = decomprasseDefault(temp_chunk);
			filterImageData(decompress_data, 3);
			break;
		}
		case Png::greyscalWithAlfa:
			break;
		case Png::truecolorWithAlfe:
		{
			auto decompress_data = decomprasseDefault(temp_chunk);
			for (const auto& i : decompress_data)
			{
				if (i == 1)
				{
					std::cout << std::bitset<8>(i) << std::endl;
				}
			}
			filterImageData(decompress_data, 4);
			break;
		}
		default:
			throw InvalidChunkDataValue;
			break;
		}
	}
}
std::vector<uint8_t> Png::decomprasseDefault(Chunk chunk)
{
	if (static_cast<uint8_t>(chunk.chunk_data[0]) < '\x18')
		throw InvalidChunkDataValue;

	std::vector<uint8_t> decompress_data;

	z_stream stream;
	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;
	stream.opaque = Z_NULL;
	stream.avail_in = static_cast<uInt>(chunk.chunk_data.size());
	stream.next_in = reinterpret_cast<Bytef*>(chunk.chunk_data.data());

	if (inflateInit(&stream) != Z_OK)
	{
		throw "ERROR INITIALIZATION ZLIB";
	}

	unsigned char* buffer = new unsigned char[chunk.length - 1];

	do {
		stream.avail_out = chunk.length;
		stream.next_out = buffer;

		int result = inflate(&stream, Z_SYNC_FLUSH);

		if (result != Z_OK && result != Z_STREAM_END)
		{
			inflateEnd(&stream);
			throw "ERROR DECOMPRESSED ZLIB";
		}

		int decompressed_bytes = chunk.length - stream.avail_out;
		decompress_data.insert(decompress_data.end(), buffer, buffer + decompressed_bytes);

	} while (stream.avail_out == 0);

	inflateEnd(&stream);

	delete[] buffer;

	return decompress_data;
}
void Png::filterImageData(std::vector<uint8_t>& image_data, uint8_t steps)
{
	switch (image_data[0])
	{
	case 0:
		for (size_t i = 1; i < image_data.size() - 1; i++)
		{
			image_data[i - 1] = image_data[i];
		}
		break;
	case 1:
		for (size_t i = steps + 1; i < image_data.size() - 1; i++)
		{
			image_data[i] = abs(image_data[i] - image_data[i - steps]);
		}
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	default:
		throw InvalidChunkDataValue;
		break;
	}
};
