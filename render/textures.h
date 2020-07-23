#pragma once
#include "../lodepng.h"
#include <winsock.h>
#include "../resource.h"
#include "../hooks/hooks.h"
#include "../Controls/Controls.h"

class cTextures {
public:
	unsigned short id = 0;
	int width = 0;
	int height = 0;

	cTextures(unsigned short id, int width, int height) {
		this->id = id;
		this->width = width;
		this->height = height;
	}

	std::uint8_t* load_image()
	{
		std::string type = "PNG";
		auto resource = FindResource(Global::dllmodule, MAKEINTRESOURCE(this->id), type.data());
		auto loaded_resource = LoadResource(Global::dllmodule, resource);
		auto resource_ptr = LockResource(loaded_resource);
		auto resource_size = SizeofResource(Global::dllmodule, resource);

		std::vector< std::uint8_t > image;

		auto w = std::uint32_t(this->width);
		auto h = std::uint32_t(this->height);

		if (const auto error = lodepng::decode(image, w, h, (unsigned char*)resource_ptr, resource_size))
			exit(0);

		const auto data = new std::uint8_t[image.size()];
		std::copy(image.begin(), image.end(), data);

		return data;
	}	
};
//extern cTextures Textures;
