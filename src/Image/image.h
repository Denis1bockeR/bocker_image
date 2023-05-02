#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <vector>
#include <variant>

#include "../ParseImage/png.h"

class Image
{
public:
	virtual uint32_t getWidth() noexcept = 0;
	virtual uint32_t getHeight() noexcept = 0;
	virtual uint8_t getBitDepth() noexcept = 0;
	virtual void changeImageColor(void (*pntFunc)(char)) = 0;
	virtual void changeImageColor(uint8_t RDiv, uint8_t GDiv, uint8_t BDiv) = 0;
};

#endif // !_IMAGE_H_
