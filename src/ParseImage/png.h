#ifndef _PNG_H_
#define _PNG_H_

#include <vector>
#include <string>
#include <fstream>

#include "../Image/image.h"

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
	PLTE = 1163152464,
	IDAT = 1413563465
};

class Png : Image
{
public:
	Png(std::string puth);
	virtual ~Png() = default;

private:
	void readChunk(std::ifstream* image, int& i) noexcept;
	inline void updatePosRead(std::ifstream* image, int& i, int newI) noexcept
	{
		i = newI;
		image->seekg(i);
	};

	struct Chunk
	{
		uint32_t length = 0;
		uint32_t chunkName;
		std::vector<std::byte> chunkData;
		uint32_t crc = 0;
	};
	enum colorVariant
	{
		greyscale = 0,
		truecolor = 2,
		indexedColor = 3,
		greyscalWithAlfa = 4,
		truecolorWithAlfe = 6
	};
};

#endif // !_PNG_H_
