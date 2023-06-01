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

	Chunk tempChunk;
	image->read(reinterpret_cast<char*>(&tempChunk.length), 4);
	tempChunk.length = _byteswap_ulong(tempChunk.length);

	updatePosRead(image, i, i + 4);
	image->read(reinterpret_cast<char*>(&tempChunk.chunk_name), 4);


	updatePosRead(image, i, i + 4);
	for (int j = 0; j < tempChunk.length; j++)
	{
		tempChunk.chunk_data.emplace_back();
	}

	image->read(reinterpret_cast<char*>(tempChunk.chunk_data.data()), tempChunk.length);
	updatePosRead(image, i, i+tempChunk.length);

	image->read(reinterpret_cast<char*>(&tempChunk.crc), 4);
	i += 4;

	switch (tempChunk.chunk_name)
	{
	case 1380206665:
		width = static_cast<uint32_t>(tempChunk.chunk_data[0]) << 24 | static_cast<uint32_t>(tempChunk.chunk_data[1]) << 16 | static_cast<uint32_t>(tempChunk.chunk_data[2]) << 8 | static_cast<uint32_t>(tempChunk.chunk_data[3]);
		height = static_cast<uint32_t>(tempChunk.chunk_data[4]) << 24 | static_cast<uint32_t>(tempChunk.chunk_data[5]) << 16 | static_cast<uint32_t>(tempChunk.chunk_data[6]) << 8 | static_cast<uint32_t>(tempChunk.chunk_data[7]);
		bit_depth = static_cast<uint8_t>(tempChunk.chunk_data[8]);
		color_type = static_cast<Png::ColorType>(tempChunk.chunk_data[9]);

		if (bit_depth != 1 or bit_depth != 2 or bit_depth != 4 or bit_depth != 8 or bit_depth != 16)
		{
			throw InvalidChunkDataValue;
		}
		break;
	case 1163152464:
		int k = 0;
		for (int j = 0; j < tempChunk.chunk_data.size(); j += 3)
		{
			data.emplace_back();
			data[k].red = static_cast<uint8_t>(tempChunk.chunk_data[j]);
			data[k].green = static_cast<uint8_t>(tempChunk.chunk_data[j+1]);
			data[k].blue = static_cast<uint8_t>(tempChunk.chunk_data[j+2]);
			k++;
		}
		break;
	case 1413563465:
		switch (color_type)
		{
		case Png::greyscale:
			char buffer[1024];
			break;
		case Png::truecolor:
			break;
		case Png::greyscalWithAlfa:
			break;
		case Png::truecolorWithAlfe:
			break;
		default:
			throw InvalidChunkDataValue;
			break;
		}
	}
}
void Png::decomprasseDefault(Chunk chunk)
{
	if (static_cast<uint8_t>(chunk.chunk_data[0]) != 78)
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

	uint32_t chunk_size = chunk.length;
	unsigned char* buffer = new unsigned char[chunk_size];

	do {
		stream.avail_out = chunk_size;
		stream.next_out = buffer;

		int result = inflate(&stream, Z_NO_FLUSH);

		if (result == Z_NEED_DICT || result == Z_DATA_ERROR || result == Z_MEM_ERROR)
		{
			inflateEnd(&stream);
			throw "ERROR DECOMPRESSED ZLIB";
		}

		int decompressedBytes = chunk_size - stream.avail_out;
		decompress_data.insert(decompress_data.end(), buffer, buffer + decompressedBytes);

	} while (stream.avail_out == 0);

	inflateEnd(&stream);
}