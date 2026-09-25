#include "AVGNG/Core/physics/PhysicsBody.hpp"
#include "AVGNG/Core/GameObject.hpp"
#include "AVGNG/Core/Debug.hpp"
#include "AVGNG/Assets/JsonUtils.hpp"
#include <imgui/imgui.h>

using namespace ng::Core;

PhysicsBody::PhysicsBody(){

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

// Update physics body
void PhysicsBody::Update(float dt){

    // Reference gameObject transform (required for all physics bodies)
    Transform* tf = owner->GetComponent<Transform>();
    if (!tf) return;


    // Update velocity using linearAcceleration
    this->SetLinearVelocity(this->linearVelocity+(this->linearAcceleration*dt));


    // Update position using linearVelocity
    glm::vec3 pos = tf->GetPosition();
    tf->SetPosition(pos + (linearVelocity * dt));

    // Update angularVelocity using angularAcceleration
    this->SetAngularAcceleration(this->angularVelocity+(this->angularAcceleration*dt));

    // Update rotation using angularVelocity
    glm::vec3 rot = tf->GetRotation();
    tf->SetRotation(rot+(angularVelocity*dt));
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
