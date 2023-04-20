#include <iostream>

#include <intrin.h>

#include "png.h"
#include <bitset>

Png::Png(std::string puth)
{
	std::ifstream image;
	image.open(puth, std::ios::in | std::ios::binary | std::ios::ate);
	int length = image.tellg();
	int i = 8;
	while (i < length)
	{
		chunks.emplace_back(readChunk(&image, i));
	}
}

Chunk Png::readChunk(std::ifstream* image, int& i) noexcept
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

	return tempChunk;
}

uint32_t Png::getWidth() noexcept
{
	if (chunks.size() != 0)
	{
		return static_cast<uint32_t>(chunks[0].chunkData[0]) << 24 | static_cast<uint32_t>(chunks[0].chunkData[1]) << 16 | static_cast<uint32_t>(chunks[0].chunkData[2]) << 8 | static_cast<uint32_t>(chunks[0].chunkData[3]);
	}
	else
	{
		return PngError::MissingChunk;
	}
}
uint32_t Png::getHeight() noexcept
{
	if (chunks.size() != 0)
	{
		return static_cast<uint32_t>(chunks[0].chunkData[4]) << 24 | static_cast<uint32_t>(chunks[0].chunkData[5]) << 16 | static_cast<uint32_t>(chunks[0].chunkData[6]) << 8 | static_cast<uint32_t>(chunks[0].chunkData[7]);
	}
	else
	{
		return PngError::MissingChunk;
	}
}
uint8_t Png::getBitDepth() noexcept
{
	if (chunks.size() != 0)
	{
		uint8_t bitDepth = static_cast<uint8_t>(chunks[0].chunkData[8]);
		if (bitDepth == 1 or bitDepth == 2 or bitDepth == 4 or bitDepth == 8 or bitDepth == 16)
			return bitDepth;
		else
			return PngError::InvalidChunkDataValue;
	}
	else
	{
		return PngError::MissingChunk;
	}
}