#include "Utils.h"

#include <memory>

namespace ImageUtils
{
	unsigned char* removeTransparency(int width, int height, unsigned char* image)
	{
		unsigned char* buffer = new unsigned char[width * height * 4];
		std::memcpy(buffer, image, sizeof(unsigned char) * width * height * 4);

		for (int i = 0; i < width * height * 4; i += 4)
			buffer[i + 3] = 255;

		return buffer;
	}
}