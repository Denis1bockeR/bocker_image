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
			chunks.emplace_back(readChunk(&image, i));
		}
	}
	else
	{
		throw(WrongFileFormaat);
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

void Png::changeImageColor(void (*pntFunc)(char))
{
	try
	{
		switch (static_cast<uint8_t>(chunks[0].chunkData[9]))
		{
		case 0:
			break;
		case 2:
			break;
		case 3:
			for (auto& i : chunks)
			{
				if (i.chunkName == chunkName::PLTE)
				{
					for (size_t j = 0; j < i.length; j++)
					{
						pntFunc(static_cast<char>(i.chunkData[j]));
					}
					break;
				}
			}
			break;
		case 4:
			break;
		case 6:
			break;
		default:
			throw(InvalidChunkDataValue);
			break;
		}
	}
	catch (...)
	{
		std::cout << "Invalid color type" << std::endl;
	}
}
void Png::changeImageColor(uint8_t RDiv, uint8_t GDiv, uint8_t BDiv)
{
	try
	{
		switch (static_cast<uint8_t>(chunks[0].chunkData[9]))
		{
		case 0:
			break;
		case 2:
			break;
		case 3:
			for (auto& i : chunks)
			{
				if (i.chunkName == chunkName::PLTE)
				{
					for (size_t j = 0; j < i.length; j+=3)
					{
						i.chunkData[j] = static_cast<std::byte>(static_cast<uint8_t>(i.chunkData[j])/RDiv);
						i.chunkData[j+1] = static_cast<std::byte>(static_cast<uint8_t>(i.chunkData[j+1]) / GDiv);
						i.chunkData[j+2] = static_cast<std::byte>(static_cast<uint8_t>(i.chunkData[j+2]) / BDiv);
					}
					break;
				}
			}
			break;
		case 4:
			break;
		case 6:
			break;
		default:
			throw(InvalidChunkDataValue);
			break;
		}
	}
	catch (...)
	{
		std::cout << "Invalid color type" << std::endl;
	}
}