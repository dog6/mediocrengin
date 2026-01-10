#pragma once

#include <AVGNG/Mesh.hpp>
#include <AVGNG/Component.hpp>
#include <AVGNG/ObjFileParser.hpp>

namespace ng::Graphics {

	class MeshRenderer : public ng::Core::Component {
		Mesh* mesh = nullptr;
	public:
		Shader* shader;

		void Draw(ng::Graphics::Camera& camera, ng::Core::Transform& transform);
		
		// Getters & Setters
		void SetMesh(ng::Graphics::Mesh* mesh);
		Mesh* GetMesh();

		void LoadMeshWithOBJPath(const char* objPath);
		void LoadMeshShader(const char* vertShaderPath, const char* fragShaderPath);
	};

}