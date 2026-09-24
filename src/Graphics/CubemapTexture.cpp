#include "AVGNG/Graphics/CubemapTexture.hpp"

namespace ng::Graphics {

	CubemapTexture::CubemapTexture() {
		// Default skybox path
		this->front_texture_path = "D:/Projects/CPP/smallengine/res/images/cubemaps/skybox0/nx.png";
		this->back_texture_path = "D:/Projects/CPP/smallengine/res/images/cubemaps/skybox0/px.png";
		this->left_texture_path = "D:/Projects/CPP/smallengine/res/images/cubemaps/skybox0/nz.png";
		this->right_texture_path = "D:/Projects/CPP/smallengine/res/images/cubemaps/skybox0/pz.png";
		this->bottom_texture_path = "D:/Projects/CPP/smallengine/res/images/cubemaps/skybox0/ny.png";
		this->top_texture_path = "D:/Projects/CPP/smallengine/res/images/cubemaps/skybox0/py.png";

	}

	CubemapTexture::CubemapTexture(std::string front, std::string back, std::string left, std::string right, std::string top, std::string bottom)
	{
		this->front_texture_path = front;
		this->back_texture_path = back;
		this->left_texture_path = left;
		this->right_texture_path = right;
		this->top_texture_path = top;
		this->bottom_texture_path = bottom;
	}

}