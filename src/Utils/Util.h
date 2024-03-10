#pragma once

#include <cstdint>
#include <utility>
#include <limits>
#include <cmath>

namespace ImageUtils
{
	constexpr std::size_t NO_PIXEL = std::numeric_limits<std::size_t>::max();

	inline std::size_t getPixelIndex(int x, int y, int width, int height, int channel = 4)
	{
		if (x < 0 || x >= width || y < 0 || y >= height)
			return NO_PIXEL;

		return (x + y * width) * channel;
	}

	inline std::pair<int, int> getCoordinates(std::size_t index, int width, int height, int channel = 4)
	{
		if (index == NO_PIXEL || index >= width * height * channel)
			return std::make_pair(-1, -1);

		int y = index / (width * channel);
		int x = (index % (width * channel)) / channel;

		return { x, y };
	}

	inline unsigned char* getPixel(int x, int y, int width, int height, unsigned char* image)
	{
		std::size_t index = getPixelIndex(x, y, width, height);

		if (index == NO_PIXEL)
			return 0;

		return image + index;
	}

	inline void setPixel(int x, int y, int r, int g, int b, int a, int width, int height, unsigned char* image)
	{
		std::size_t baseIndex = getPixelIndex(x, y, width, height);

		if (baseIndex == NO_PIXEL)
			return;

		image[baseIndex + 0] = r;
		image[baseIndex + 1] = g;
		image[baseIndex + 2] = b;
		image[baseIndex + 3] = a;
	}
}