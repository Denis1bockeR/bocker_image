#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <vector>
#include <variant>

#include "../ParseImage/png.h"

class Image
{
public:
	uint32_t getWidth() noexcept { return width; };
	uint32_t getHeight() noexcept { return height; };
	uint8_t getBitDepth() noexcept { return bitDepth; };
	virtual void changeImageColor(void (*pntFunc)(char)) = 0;
	virtual void changeImageColor(uint8_t RDiv, uint8_t GDiv, uint8_t BDiv) = 0;
protected:
	uint32_t width, height;
	uint8_t bitDepth;

	struct RGBA
	{
		uint8_t red = 0;
		uint8_t green = 0;
		uint8_t blue = 0;
		uint8_t alfa = 0;
	};

	std::vector<RGBA> data;
};

#endif // !_IMAGE_H_
