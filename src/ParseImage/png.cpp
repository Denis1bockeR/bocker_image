#include <fstream>
#include <intrin.h>

#include "png.h"

Png::Png(std::string puth)
{
	std::ifstream image;
	image.open(puth, std::ios::in | std::ios::binary | std::ios::ate);
	int i = 64;
	while (i < image.end)
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
	updatePosRead(image, i, tempChunk.length);

	image->read(reinterpret_cast<char*>(&tempChunk.crc), 4);
	i += 4;

	return tempChunk;
}