#ifndef _IMAGEPARSER_H_
#define _IMAGEPARSER_H_

#include <string>
#include <fstream>

class ImageParser
{
public:
	ImageParser() = delete;
	ImageParser(const std::string& puth);
	~ImageParser() = default;
private:
	void bmpParser(const std::string& puth) noexcept;
	void pngParser() noexcept;
	void jpgParser() noexcept;
};

#endif // !_IMAGEPARSER_H_
