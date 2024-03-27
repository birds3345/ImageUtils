#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "Utils/Utils.h"

#define IMAGEUTILS_REQUIRES(str, t) (args.contains(str) && args[str].type == Arg::Type::t)

struct Arg
{
	enum class Type
	{
		Flag,
		Arg,
		Value,
	};

	Type type;

	std::string value;
};

int main(int argc, const char* argv[])
{
	if (argc == 1)
	{
		std::cout << "type 'imageutils --help' for help" << std::endl;

		return 0;
	}

	std::unordered_map<std::string, Arg> args;

	for (int i = 0; i < argc; i++)
	{
		std::string arg(argv[i]);

		if (arg.starts_with("--"))
		{
			args[arg.substr(2)] = { Arg::Type::Flag };
		}
		else if (arg.starts_with("-"))
		{
			if (i + 1 >= argc)
			{
				std::cout << "an argument '" << arg << "' must have a value" << std::endl;

				return 0;
			}

			args[arg.substr(1)] = { Arg::Type::Arg, std::string(argv[i + 1])};
		}
		else
		{
			args[arg] = { Arg::Type::Value };
		}
	}

	if (IMAGEUTILS_REQUIRES("help", Flag))
	{
		std::cout << "imagebleed -in (input file) -out (output file)" << std::endl;
		std::cout << "removetransparency -in (input file) -out (output file)" << std::endl;
		std::cout << "monochrome -in (input file) -out (output file)" << std::endl;

		return 0;
	}

	if (std::string(argv[1]).starts_with("-"))
	{
		std::cout << "you must specify a command" << std::endl;

		return 0;
	}

	if (!IMAGEUTILS_REQUIRES("in", Arg))
	{
		std::cout << "imageutils requires an '-in' argument" << std::endl;

		return 0;
	}

	std::string inputFile = std::move(args["in"].value);
	std::string outputFile = std::move(args["out"].value);
	args.erase("in");
	args.erase("out");

	if (outputFile == "")
	{
		std::size_t last = inputFile.find_last_of('.');

		if (last == std::string::npos)
		{
			std::cout << "could not determine output file" << std::endl;

			return 0;
		}

		outputFile = inputFile.substr(0, last) + "_out.png";
	}

	int width;
	int height;
	int channels;

	unsigned char* image = stbi_load(inputFile.c_str(), &width, &height, &channels, 4);

	if (stbi_failure_reason())
	{
		std::cout << "failed to load: " << stbi_failure_reason() << std::endl;

		return 0;
	}

	unsigned char* output;

	if (std::strcmp(argv[1], "imagebleed") == 0)
		output = ImageUtils::bleedImage(width, height, image);

	else if (std::strcmp(argv[1], "removetransparency") == 0)
		output = ImageUtils::removeTransparency(width, height, image);
	
	else if (std::strcmp(argv[1], "monochrome") == 0)
		output = ImageUtils::monochrome(width, height, image);

	else
	{
		std::cout << argv[1] << " is not a valid command" << std::endl;

		return 0;
	}

	stbi_write_png(outputFile.c_str(), width, height, 4, output, width * 4);
	
	if (stbi_failure_reason())
	{
		std::cout << "failed to write: " << stbi_failure_reason() << std::endl;

		return 0;
	}

	delete[] output;
	STBI_FREE(image);
}