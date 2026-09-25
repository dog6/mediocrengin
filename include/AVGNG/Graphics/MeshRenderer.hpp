#pragma once

#include "AVGNG/Core/IComponent.hpp"
#include "AVGNG/Core/Transform.hpp"

#include <filesystem>

// Forward declare ALL underlying pointer types inside their exact matching nested namespaces:
namespace ng::Graphics {
	class Mesh; 
	class Texture;
	class Camera;
}
namespace ng::Graphics {

	class Mesh; 

	class MeshRenderer : public ng::Core::IComponent {
		
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

		void Save(nlohmann::json& j) override;
		void Load(const nlohmann::json& j) override;

	};

}