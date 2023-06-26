#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <vector>
#include <variant>

struct RGBA
{
	uint8_t red = 0;
	uint8_t green = 0;
	uint8_t blue = 0;
	uint8_t alfa = 0;
};

class Image
{
public:
	uint32_t getWidth() noexcept { return width; };
	uint32_t getHeight() noexcept { return height; };
	uint8_t getBitDepth() noexcept { return bit_depth; };

	std::vector<RGBA> getPixelData() noexcept { return data; };
protected:
	uint32_t width, height;
	uint8_t bit_depth;

	std::vector<RGBA> data;

	inline void updatePosRead(std::ifstream* image, int& i, int newI) noexcept
	{
		i = newI;
		image->seekg(i);
	};
};

#endif // !_IMAGE_H_
