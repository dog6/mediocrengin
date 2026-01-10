#pragma once

#include <AVGNG/Mesh.hpp>
#include <AVGNG/Component.hpp>

namespace ng::Graphics {

	class MeshRenderer : public ng::Core::Component {
	public:

		Mesh* mesh = nullptr;
		Shader* shader;

		void Draw(Camera& camera, ng::Core::Transform& transform);
        

	};

}