#pragma once

#include <AVGNG/Mesh.hpp>
#include <AVGNG/ObjFileParser.hpp>
#include <AVGNG/Component.hpp>

namespace ng::Graphics {

	class MeshRenderer : public ng::Core::Component {
		Mesh* mesh = nullptr;
	public:
		Shader* shader;

		void Draw(ng::Graphics::Camera& camera, ng::Core::Transform& transform);
		
		// Getters & Setters
		void SetMesh(ng::Graphics::Mesh* mesh);
		Mesh* GetMesh();

		void LoadMesh(const char* objPath);
		void LoadShader(const char* shaderName, const char* vertShaderPath, const char* fragShaderPath);

		void OnInspectorGUI() override;


	};

}