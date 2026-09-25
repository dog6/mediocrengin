#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <nlohmann/json.hpp>
#include "AVGNG/Core/GameObject.hpp"
#include "AVGNG/Core/IComponent.hpp"
#include "AVGNG/Core/Transform.hpp"

namespace ng::Core {

	// Describes basic rigidbody physic gameobjects
	class PhysicsBody : public IComponent {

        Transform* tf;
        glm::vec3 linearVelocity = glm::vec3();
		glm::vec3 linearAcceleration = glm::vec3(0,0,0);

		glm::vec3 angularVelocity = glm::vec3(0,0,0);
		glm::vec3 angularAcceleration = glm::vec3(0,0,0);

	public:

        PhysicsBody();
        ~PhysicsBody();

		void SetLinearVelocity(glm::vec3 velocity);
		void SetLinearAcceleration(glm::vec3 accel);
		glm::vec3 GetLinearVelocity() { return this->linearVelocity; }
		glm::vec3 GetLinearAcceleration() { return this->linearAcceleration; }

		void SetAngularVelocity(glm::vec3 velocity);
		void SetAngularAcceleration(glm::vec3 accel);
		glm::vec3 GetAngularVelocity() { return this->angularVelocity; }
		glm::vec3 GetAngularAcceleration() { return this->angularAcceleration; }

  		void Update(float deltaTime);
		void OnInspectorGUI() override;

		void Save(nlohmann::json& j) override;
		void Load(const nlohmann::json& j) override;
		
	};

}
