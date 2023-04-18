#ifndef _PNG_H_
#define _PNG_H_

#include <vector>
#include <string>

struct Chunk
{
	uint32_t length = 0;
	uint32_t chunkName = 0;
	std::vector<std::byte> chunkData;
	uint32_t crc = 0;
};

class Png
{
public:
	Png(std::string puth);
	~Png() = default;

	constexpr uint32_t getWidth() noexcept;
	constexpr uint32_t Height() noexcept;
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
