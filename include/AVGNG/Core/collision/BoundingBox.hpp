#pragma once

#include <glm/glm.hpp>
#include "AVGNG/Core/Transform.hpp"
#include "AVGNG/Core/IComponent.hpp"
#include "AVGNG/Graphics/Mesh.hpp"

namespace ng::Core {

	// Describes basic bounding box of a mesh
	class BoundingBox : public IComponent  {

		Transform* tf;
		ng::Graphics::Mesh* mesh;
        glm::vec3 bbMin, bbMax; // AABB colliders, 2 corners describe box

		// Settings to show boundingbox as gizmo
		glm::vec3 gizmoColor;
		bool gizmoVisible;

	public:

		BoundingBox();
		~BoundingBox();

		void Draw(ng::Graphics::Camera& cam, ng::Core::Transform& tf);
		void ComputeFromVertices(const std::vector<ng::Graphics::Vertex>& vertices);
		bool Overlaps(const glm::vec3& minA, const glm::vec3& maxA, const glm::vec3& minB, const glm::vec3& maxB);

	};

}
