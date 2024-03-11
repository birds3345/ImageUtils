#include "Utils.h"

#include <memory>

namespace ImageUtils
{
	unsigned char* monochrome(int width, int height, unsigned char* image)
	{
		unsigned char* buffer = new unsigned char[width * height * 4];
		std::memcpy(buffer, image, sizeof(unsigned char) * width * height * 4);

		for (int i = 0; i < width * height * 4; i += 4)
		{
			unsigned char color = (buffer[i + 0] + buffer[i + 1] + buffer[i + 2]) / 3;

			buffer[i + 0] = color;
			buffer[i + 1] = color;
			buffer[i + 2] = color;
		}

		return buffer;
	}
}