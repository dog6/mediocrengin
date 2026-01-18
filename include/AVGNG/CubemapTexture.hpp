#pragma once

#include <string>

namespace ng::Graphics {

	struct CubemapTexture {

	public:
		CubemapTexture();
		CubemapTexture(std::string front, std::string back,
			std::string left, std::string right,
			std::string top, std::string bottom);

		unsigned int textureID;
		std::string front_texture_path;
		std::string back_texture_path;
		std::string left_texture_path;
		std::string right_texture_path;
		std::string top_texture_path;
		std::string bottom_texture_path;


	};
}