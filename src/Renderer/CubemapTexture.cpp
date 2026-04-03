#include "AVGNG/Renderer/CubemapTexture.hpp"

namespace ng::Graphics {

	CubemapTexture::CubemapTexture() {
		// Default skybox path
		this->front_texture_path = "D:/Projects/CPP/smallengine/res/images/skyboxes/cloudy/blue/bluecloud_ft.jpg";
		this->back_texture_path = "D:/Projects/CPP/smallengine/res/images/skyboxes/cloudy/blue/bluecloud_bk.jpg";
		this->left_texture_path = "D:/Projects/CPP/smallengine/res/images/skyboxes/cloudy/blue/bluecloud_lf.jpg";
		this->right_texture_path = "D:/Projects/CPP/smallengine/res/images/skyboxes/cloudy/blue/bluecloud_rt.jpg";
		this->top_texture_path = "D:/Projects/CPP/smallengine/res/images/skyboxes/cloudy/blue/bluecloud_dn.jpg";
		this->bottom_texture_path = "D:/Projects/CPP/smallengine/res/images/skyboxes/cloudy/blue/bluecloud_up.jpg";
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