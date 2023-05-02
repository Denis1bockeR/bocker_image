#ifndef _PNG_H_
#define _PNG_H_

#include <vector>
#include <string>
#include <fstream>

#include "../Image/image.h"

struct Chunk
{
	uint32_t length = 0;
	uint32_t chunkName;
	std::vector<std::byte> chunkData;
	uint32_t crc = 0;
};

enum PngError
{
	MissingChunk = -1,
	WrongChunkType = -2,
	InvalidChunkDataValue = -3,
	WrongFileFormaat = -4
};

enum chunkName
{
	IHDR = 1380206665,
	sRGB = 1111970419,
	pHYs = 1935231088,
	PLTE = 1163152464
};

class Png : Image
{
public:
	Png(std::string puth);
	virtual ~Png() = default;

	virtual uint32_t getWidth() noexcept;
	virtual uint32_t getHeight() noexcept;
	virtual uint8_t getBitDepth() noexcept;
	virtual void changeImageColor(void (*pntFunc)(char));
	virtual void changeImageColor(uint8_t RDiv, uint8_t GDiv, uint8_t BDiv);
private:
	std::vector<Chunk> chunks;

	Chunk readChunk(std::ifstream* image, int& i) noexcept;
	inline void updatePosRead(std::ifstream* image, int& i, int newI) noexcept
	{
		i = newI;
		image->seekg(i);
	};
};

#endif // !_PNG_H_
