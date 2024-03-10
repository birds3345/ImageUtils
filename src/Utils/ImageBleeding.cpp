#include "Utils.h"

#include "Util.h"

#include <unordered_set>
#include <queue>
#include <iostream>

namespace ImageUtils
{
	unsigned char* bleedImage(int width, int height, unsigned char* image)
	{
		unsigned char* buffer = new unsigned char[width * height * 4];
		std::memcpy(buffer, image, sizeof(unsigned char) * width * height * 4);
		
		bool* visited = new bool[width * height] {};

		std::queue<size_t> queue;
		
		//fill the queue with indices that are transparent and adjacent to a non transparent pixel
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				unsigned char* pixel = getPixel(i, j, width, height, image);
				
				if (pixel[3] != 0) continue;
				
				//check for a opague pixel
				for (int a = -1; a <= 1; a++)
				{
					bool has = false;

					for (int b = -1; b <= 1; b++)
					{
						if (a == 0 && b == 0) continue;

						std::size_t index = getPixelIndex(i + a, j + b, width, height);
						if (index == NO_PIXEL) continue;

						unsigned char* pixel = image + index;
						if (pixel[3] == 0) continue;
						
						//found a non transparent pixel 
						visited[getPixelIndex(i, j, width, height, 1)] = true;
						queue.push(getPixelIndex(i, j, width, height));

						has = true;

						break;
					}

					if (has) break;
				}
			}
		}
		
		//ensures that iterator order does not affect the output image
		std::unordered_set<int> breadth;
		
		//run bfs
		while (!queue.empty())
		{
			breadth.clear();

			std::size_t size = queue.size();

			for (int i = 0; i < size; i++)
			{
				auto [x, y] = getCoordinates(queue.front(), width, height);

				breadth.insert(getPixelIndex(x, y, width, height, 1));
			}

			for (int i = 0; i < size; i++)
			{
				auto [x, y] = getCoordinates(queue.front(), width, height);

				//an unsigned char may not have enough room to store the addition of all adjacent pixels
				int rC = 0;
				int gC = 0;
				int bC = 0;
				int count = 0;

				for (int a = -1; a <= 1; a++)
				{
					for (int b = -1; b <= 1; b++)
					{
						if (a == 0 && b == 0) continue;

						std::size_t index = getPixelIndex(x + a, y + b, width, height);
						if (index == NO_PIXEL) continue;

						std::size_t visitedIndex = getPixelIndex(x + a, y + b, width, height, 1);

						if ((buffer[index + 0] != 0 || buffer[index + 1] != 0 || buffer[index + 2] != 0) && !breadth.contains(visitedIndex))
						{
							//adding the colors to later be averaged together
							rC += buffer[index + 0];
							gC += buffer[index + 1];
							bC += buffer[index + 2];

							count++;
						}

						if (image[index + 3] == 0 && !visited[visitedIndex])
						{
							//add unvisited transparent pixels into the queue to be processed
							queue.push(index);

							visited[visitedIndex] = true;
						}
					}
				}

				if (count > 0)
				{
					buffer[queue.front() + 0] = rC / count;
					buffer[queue.front() + 1] = gC / count;
					buffer[queue.front() + 2] = bC / count;
				}

				queue.pop();
			}
		}

		delete[] visited;

		return buffer;
	}
}