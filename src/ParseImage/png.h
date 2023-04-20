#ifndef _PNG_H_
#define _PNG_H_

#include <vector>
#include <string>
#include <fstream>

struct Chunk
{
	uint32_t length = 0;
	uint32_t chunkName = 0;
	std::vector<std::byte> chunkData;
	uint32_t crc = 0;
};

enum PngError
{
	MissingChunk = -1,
	WrongChunkType = -2,
	InvalidChunkDataValue = -3
};

class Png
{
public:
	Png(std::string puth);
	~Png() = default;

	uint32_t getWidth() noexcept;
	uint32_t getHeight() noexcept;
	uint8_t getBitDepth() noexcept;
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
