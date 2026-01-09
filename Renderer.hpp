#pragma once

#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <vector>
#include "Mesh.hpp"

namespace ng {
namespace Graphics {


	// Renders Mesh objects to a GLFW instance
	class Renderer {
	public:
		static void Render(std::vector<Mesh*> meshes);
	};




}}

#endif