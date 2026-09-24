#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <nlohmann/json.hpp>
#include "AVGNG/Core/GameObject.hpp"
#include "AVGNG/Core/IColliderShape.hpp"
#include "AVGNG/Core/Transform.hpp"

namespace ng::Core {

	// Describes basic rigidbody physic gameobjects
	class SphereCollider : public IComponent, IColliderShape  {

		float radius; // radius of sphere

	public:

        SphereCollider();
        ~SphereCollider();
		
		void SetRadius(float r) { this->radius = r; }
		float GetRadius() { return this->radius; }

		void OnInspectorGUI() override;

	};

}
