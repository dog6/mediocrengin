#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <nlohmann/json.hpp>
// #include "AVGNG/Core/GameObject.hpp"
#include "AVGNG/Core/IComponent.hpp"
#include "AVGNG/Core/Transform.hpp"

namespace ng::Core {

	// Describes basic rigidbody physic gameobjects
	class PhysicsBody : public IComponent {

        Transform* tf = nullptr;

		glm::vec3 internalVelocity = glm::vec3(); // for internal physics calculations

        glm::vec3 linearVelocity = glm::vec3(); // external velocities
		glm::vec3 linearAcceleration = glm::vec3(0,0,0);

		glm::vec3 angularVelocity = glm::vec3(0,0,0);
		glm::vec3 angularAcceleration = glm::vec3(0,0,0);
		glm::vec3 gravityForce = glm::vec3(0,0,0); // force of gravity on object
		
		float restitution = 0; // "bounciness" of physics body (0-1)
		float friction = 0; // force needed to start moving
		float mass = 1; // weight of physics body

	public:

        PhysicsBody();
        ~PhysicsBody();

		void HandlePhysics();

		// Getters & Setters

		void SetLinearVelocity(glm::vec3 velocity);
		void SetLinearAcceleration(glm::vec3 accel);
		glm::vec3 GetLinearVelocity() { return this->linearVelocity; }
		glm::vec3 GetLinearAcceleration() { return this->linearAcceleration; }

		void SetAngularVelocity(glm::vec3 velocity);
		void SetAngularAcceleration(glm::vec3 accel);
		glm::vec3 GetAngularVelocity() { return this->angularVelocity; }
		glm::vec3 GetAngularAcceleration() { return this->angularAcceleration; }

		void SetRestitution(float r) { this->restitution = r; }
		float GetRestitution() { return this->restitution; }

		void SetFriction(float f) { this->friction = f; }
		float GetFriction() { return this->friction; }

		void SetMass(float m) { this->mass = m; }
		float GetMass() { return this->mass; }

		void SetGravity(glm::vec3 g) { this->gravityForce = g;}
		glm::vec3 GetGravity() { return this->gravityForce; }


		
		// Inherited methods

  		void Update(float deltaTime);
		void OnInspectorGUI() override;

		void Save(nlohmann::json& j) override;
		void Load(const nlohmann::json& j) override;
		
	};

}
