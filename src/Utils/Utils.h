#pragma once

namespace ImageUtils
{
	unsigned char* bleedImage(int width, int height, unsigned char* input);
	unsigned char* removeTransparency(int width, int height, unsigned char* input);
	unsigned char* monochrome(int width, int height, unsigned char* input);
}