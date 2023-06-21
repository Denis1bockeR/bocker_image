#ifndef _PNG_H_
#define _PNG_H_

#include <vector>
#include <string>
#include <fstream>
#include <functional>

#include "image.h"

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
	struct Chunk
	{
		uint32_t length = 0;
		uint32_t chunk_name;
		std::vector<std::byte> chunk_data;
		uint32_t crc = 0;
	};

	void readChunk(std::ifstream* image, int& i) noexcept;
	inline void updatePosRead(std::ifstream* image, int& i, int newI) noexcept
	{
		i = newI;
		image->seekg(i);
	};
	std::vector<uint8_t> decomprasseDefault(Chunk chunk);
	std::vector<uint8_t> filterImageData(std::vector<uint8_t>& image_data, uint8_t steps);
	uint8_t paethPredictor(uint8_t a, uint8_t b, uint8_t c) noexcept;
	void readData(std::vector<uint8_t> data, uint8_t steps) noexcept;

	enum ColorType
	{
		greyscale = 0,
		truecolor = 2,
		indexedColor = 3,
		greyscalWithAlfa = 4,
		truecolorWithAlfe = 6
	};
	ColorType color_type;
};

#endif // !_PNG_H_
