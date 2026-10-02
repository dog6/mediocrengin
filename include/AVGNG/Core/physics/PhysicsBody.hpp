#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <nlohmann/json.hpp>
// #include "AVGNG/Core/GameObject.hpp"
#include "AVGNG/Core/IComponent.hpp"
#include "AVGNG/Core/Transform.hpp"

namespace ng::Core {

	// Forward declaration. The header only stores a pointer to a Collider.
	// PhysicsBody.cpp includes the full definition.
	class Collider;

	// Describes basic rigidbody physic gameobjects
	class PhysicsBody : public IComponent {

        Transform* tf = nullptr;

		bool isKinematic = false; // ignores external forces
        glm::vec3 linearVelocity = glm::vec3(); // external velocities
		glm::vec3 linearAcceleration = glm::vec3(0,0,0);

		glm::vec3 angularVelocity = glm::vec3(0,0,0);
		glm::vec3 angularAcceleration = glm::vec3(0,0,0);
		glm::vec3 gravityForce = glm::vec3(0,0,0); // force of gravity on object
		
		float restitution = 0; // "bounciness" of physics body (0-1)
		float friction = 0; // force needed to start moving
		float mass = 1; // weight of physics body

		// Moves the owner by "delta". Used to push bodies out of an overlap.
		void ApplyPositionCorrection(const glm::vec3& delta);
		bool sleeping = false;           // true = the body skips Update
		bool touchedThisFrame = false;   // true = HandlePhysics found a contact in this frame
		float sleepTimer = 0.0f;         // time that the body was slow and in contact

		// Data that the body records when it falls asleep.
		// ShouldWake() compares the current state with this data.
		glm::vec3  sleepPosition{ 0.0f };          // world position of the body
		Collider*  supportCollider  = nullptr;     // collider that the body rests on
		Transform* supportTransform = nullptr;     // transform of the support
		glm::vec3  supportPosition{ 0.0f };        // world position of the support

		void UpdateSleepState(float dt);
		void RecordSleepState();   // saves the sleep data. Runs when the body falls asleep.
		bool ShouldWake();         // runs each frame while the body sleeps
	public:

        PhysicsBody();
        ~PhysicsBody();
		
		void Wake() { sleeping = false; sleepTimer = 0.0f; }
		bool IsSleeping() const { return sleeping; }

		// Player helpers
		void SetHorizontalVelocity(float x, float z); // keeps the vertical velocity
		bool IsGrounded();                            // true when the contact normal points down to a floor
		bool Jump(float speed);                       // sets the vertical velocity only when grounded
		void HandlePhysics();
		void HandleTwoPhysicsBodyCollision(PhysicsBody* otherPB, const glm::vec3& normal, float penetration); // physicsBody vs physicsBody
		void HandleOnePhysicsBodyCollision(const glm::vec3& normal, float penetration); // physicsBody vs static collider

		// Gives 0 for kinematic bodies and for bodies with a mass of 0 or less.
		// A body with an inverse mass of 0 does not move in a collision.
		float GetInverseMass() const {
			return (isKinematic || mass <= 0.0f) ? 0.0f : 1.0f / mass;
		}

		// Getters & Setters

		void SetLinearVelocity(glm::vec3 velocity);
		void SetLinearAcceleration(glm::vec3 accel);
		glm::vec3 GetLinearVelocity() const { return this->linearVelocity; }
		glm::vec3 GetLinearAcceleration() const { return this->linearAcceleration; }

		void SetAngularVelocity(glm::vec3 velocity);
		void SetAngularAcceleration(glm::vec3 accel);
		glm::vec3 GetAngularVelocity() const { return this->angularVelocity; }
		glm::vec3 GetAngularAcceleration() const { return this->angularAcceleration; }

		void SetRestitution(float r) { this->restitution = r; }
		float GetRestitution() const { return this->restitution; }

		void SetFriction(float f) { this->friction = f; }
		float GetFriction() const { return this->friction; }

		void SetMass(float m) { this->mass = m; }
		float GetMass() const { return this->mass; }

		// A change of gravity wakes the body. Without this, a sleeping body
		// ignores a new gravity value until a wake test succeeds.
		void SetGravity(glm::vec3 g) { this->gravityForce = g; Wake(); }
		glm::vec3 GetGravity() const { return this->gravityForce; }

		// Inherited methods
  		void Update(float deltaTime);
		void OnInspectorGUI() override;

		void Save(nlohmann::json& j) override;
		void Load(const nlohmann::json& j) override;
		
	};

}