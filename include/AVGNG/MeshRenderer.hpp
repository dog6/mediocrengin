#pragma once

#include <AVGNG/Mesh.hpp>
#include <AVGNG/Component.hpp>
#include <filesystem>

namespace ng::Graphics {

	class MeshRenderer : public ng::Core::Component {
		Mesh* mesh = nullptr;
		Texture* diffuseTexture = nullptr;
		Texture* specularTexture = nullptr;
		Texture* normalTexture = nullptr;
		Texture* emissiveTexture = nullptr;
		Texture* alphaTexture = nullptr;
	public:
		void Draw(ng::Graphics::Camera& camera, ng::Core::Transform& transform);
		
		// Getters & Setters
		void SetMesh(ng::Graphics::Mesh* mesh);
		Mesh* GetMesh();

		void LoadMesh(const char* objPath);

		void OnInspectorGUI() override;


	};

}