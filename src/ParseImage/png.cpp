#include <iostream>
#include <bitset>
#include <cstring>

#include <intrin.h>

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
	image->read(reinterpret_cast<char*>(&tempChunk.chunkName), 4);


	updatePosRead(image, i, i + 4);
	for (int j = 0; j < tempChunk.length; j++)
	{
		tempChunk.chunkData.emplace_back();
	}

	image->read(reinterpret_cast<char*>(tempChunk.chunkData.data()), tempChunk.length);
	updatePosRead(image, i, i+tempChunk.length);

	image->read(reinterpret_cast<char*>(&tempChunk.crc), 4);
	i += 4;

	switch (tempChunk.chunkName)
	{
	case 1380206665:
		width = static_cast<uint32_t>(tempChunk.chunkData[0]) << 24 | static_cast<uint32_t>(tempChunk.chunkData[1]) << 16 | static_cast<uint32_t>(tempChunk.chunkData[2]) << 8 | static_cast<uint32_t>(tempChunk.chunkData[3]);
		height = static_cast<uint32_t>(tempChunk.chunkData[4]) << 24 | static_cast<uint32_t>(tempChunk.chunkData[5]) << 16 | static_cast<uint32_t>(tempChunk.chunkData[6]) << 8 | static_cast<uint32_t>(tempChunk.chunkData[7]);
		bitDepth = static_cast<uint8_t>(tempChunk.chunkData[8]);

		if (bitDepth != 1 or bitDepth != 2 or bitDepth != 4 or bitDepth != 8 or bitDepth != 16)
		{
			throw InvalidChunkDataValue;
		}
		break;
	case 1163152464:
		int k = 0;
		for (int j = 0; j < tempChunk.chunkData.size(); j += 3)
		{
			data.emplace_back();
			data[k].red = static_cast<uint8_t>(tempChunk.chunkData[j]);
			data[k].green = static_cast<uint8_t>(tempChunk.chunkData[j+1]);
			data[k].blue = static_cast<uint8_t>(tempChunk.chunkData[j+2]);
			k++;
		}
		break;
	case 1413563465:

	}
}