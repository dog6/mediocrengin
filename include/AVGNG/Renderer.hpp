#pragma once

#include <vector>
#include <AVGNG/Mesh.hpp>

namespace ng::Graphics {

	// Renders Mesh objects to a GLFW instance
	class Renderer {
	public:
		static void Render(std::vector<Mesh*> meshes);
	};


}
