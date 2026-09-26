#include "AVGNG/Core/physics/PhysicsBody.hpp"
#include "AVGNG/Core/GameObject.hpp"
#include "AVGNG/Core/Debug.hpp"
#include "AVGNG/Assets/JsonUtils.hpp"
#include <imgui/imgui.h>

using namespace ng::Core;

PhysicsBody::PhysicsBody(){
    if (this->owner != nullptr) {
        this->tf = this->owner->GetComponent<Transform>();
    }
    this->linearVelocity = glm::vec3(0,0,0);
}

PhysicsBody::~PhysicsBody(){}

void PhysicsBody::OnInspectorGUI() {
    
    ImGui::Text("PhysicsBody Component [%p]", this);

    if (ImGui::DragFloat3("Linear Velocity", glm::value_ptr(linearVelocity))) {
            static_cast<PhysicsBody*>(this)->SetLinearVelocity(linearVelocity);
    }

    if (ImGui::DragFloat3("Linear Acceleration", glm::value_ptr(linearAcceleration))) {
            static_cast<PhysicsBody*>(this)->SetLinearAcceleration(linearAcceleration);
    }

    if (ImGui::DragFloat3("Angular Velocity", glm::value_ptr(angularVelocity))) {
            static_cast<PhysicsBody*>(this)->SetAngularVelocity(angularVelocity);
    }

    if (ImGui::DragFloat3("Angular Acceleration", glm::value_ptr(angularAcceleration))) {
            static_cast<PhysicsBody*>(this)->SetAngularAcceleration(angularAcceleration);
    }

}

void PhysicsBody::HandlePhysics(){

    Collider* ownedCol = this->owner->GetComponent<Collider>();
    if (ownedCol) {
        Debug::Log(DEV, "PhysicsBody owns Collider '%s'", ownedCol);
        // Check for & handle collisions
        if (ownedCol->IsOverlapping()) {
            
            // Check what we're colliding with:
            Collider* other = ownedCol->GetOtherCollider();
            if (other != nullptr) {
                
                Debug::Log(DEV, "PhysicsBody Collider is overlapping other Collider '%s'", other);
                
                // Determine if other collider also has a physics body
                PhysicsBody* otherPB = other->owner->GetComponent<PhysicsBody>();
                if (otherPB != nullptr) {
                    
                    Debug::Log(DEV, "Other Collider also has a PhysicsBody component");

                    // 2 physicsbodies colliding (Dynamic - Dynamic)
                    // Compute combined restitution (bounciness)
                    float e = std::min(this->GetRestitution(), otherPB->GetRestitution());

                    // Calculate relative velocity along direction of motion
                    glm::vec3 relVel = this->GetLinearVelocity() - otherPB->GetLinearVelocity();

                    // Apply impulse response adjusted for masses
                    float m1 = this->GetMass();
                    float m2 = otherPB->GetMass();
                    float totalMass = m1 + m2;

                    if (totalMass > 0.0f) {
                        glm::vec3 v1 = this->GetLinearVelocity();
                        glm::vec3 v2 = otherPB->GetLinearVelocity();

                        // Conservation of momentum velocity exchange with restitution
                        this->SetLinearVelocity(-e * v1 * (m2 / totalMass));
                        otherPB->SetLinearVelocity(e * v2 * (m1 / totalMass));
                    }
                } else {
                    // 1 physicsbody colliding with a static collider (Dynamic - Static)
                    // Reverse linear velocity along movement axis scaled by restitution coefficient
                    Debug::Log(DEV, "Other Collider is static");

                    float e = this->GetRestitution();
                    this->SetLinearVelocity(-this->GetLinearVelocity() * e);
                }
            }
        }
    }

}

// Update physics body
void PhysicsBody::Update(float dt) {

    // Reference gameObject transform (required for all physics bodies)
    // Cache transform if member pointer is nil
    if (!this->tf) {
        if (this->owner != nullptr){ 
            this->tf = this->owner->GetComponent<Transform>();
            if (!this->tf) return;
        }
    }

    // Update linearAcceleration using gravity force
    this->SetLinearAcceleration(this->gravityForce);

    // Update velocity using acceleration: v = v0 + a * dt
    this->SetLinearVelocity(this->linearVelocity + (this->linearAcceleration * dt));

    // Check if this physics body has a collider
    HandlePhysics();

    // Update position using velocity: pos = pos0 + v * dt
    glm::vec3 pos = this->tf->GetPosition();
    this->tf->SetPosition(pos + (this->linearVelocity * dt));

    // Update angular velocity using angular acceleration: w = w0 + alpha * dt
    this->SetAngularVelocity(this->angularVelocity + (this->angularAcceleration * dt));

    // Update rotation using angular velocity: rot = rot0 + w * dt
    glm::vec3 rot = this->tf->GetRotation();
    this->tf->SetRotation(rot + (this->angularVelocity * dt));
}


// Setters

void ng::Core::PhysicsBody::SetLinearVelocity(glm::vec3 velocity)
{

    this->linearVelocity = velocity;

}

void ng::Core::PhysicsBody::SetLinearAcceleration(glm::vec3 accel)
{
    this->linearAcceleration = accel;
}

void ng::Core::PhysicsBody::SetAngularVelocity(glm::vec3 velocity)
{

    this->angularVelocity = velocity;

}

void ng::Core::PhysicsBody::SetAngularAcceleration(glm::vec3 accel)
{
    this->angularAcceleration = accel;
}


// Saving & Loading

void PhysicsBody::Save(nlohmann::json &j)
{
        Debug::Log(DEBUG, "Saving PhysicsBody %p...", this);

		// Save physics body data
        j["physicsbody"]["linearVelocity"] = ng::Assets::Vec3ToJson(linearVelocity);
		Debug::Log(DEBUG, "Finished saving PhysicsBody %p", this);

}

void PhysicsBody::Load(const nlohmann::json &j)
{
        if (!j.contains("physicsbody")) return;
		const nlohmann::json& t = j.at("physicsbody");

        linearVelocity = ng::Assets::ReadVec3(t, "linearVelocity", linearVelocity);
}
