#ifndef _IMAGEDECODER_H_
#define _IMAGEDECODER_H_

#include <string>
#include <fstream>

class ImageDecoder
{
public:
	ImageDecoder() = delete;
	ImageDecoder(const std::string& puth);
	~ImageDecoder() = default;
private:
	void bmpParser(const std::string& puth) noexcept;
	void pngParser() noexcept;
	void jpgParser() noexcept;
};

#endif // !_IMAGEPARSER_H_
