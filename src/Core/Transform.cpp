#include "AVGNG/Core/Transform.hpp"
#include "AVGNG/Core/Debug.hpp"
#include "AVGNG/Assets/JsonUtils.hpp"
#include <imgui/imgui.h>

using namespace ng::Core;


Transform::Transform()
{
	this->SetPosition(glm::vec3(0));
	this->SetRotation(glm::vec3(0));
	this->SetScale(glm::vec3(1));
}

Transform::~Transform() {}

void Transform::SetPosition(glm::vec3 pos) {
		this->position = pos;
	}

	void Transform::SetRotation(glm::vec3 rot) {
		this->rotation = rot;
	}

	void Transform::SetScale(glm::vec3 scale) {
		this->scale = scale;
	}

	void Transform::OnInspectorGUI() {

		// Render imgui elements for Transform Component
		Transform* tf = static_cast<Transform*>(this);
		glm::vec3 tfPos = tf->GetPosition();
		glm::vec3 tfRot = tf->GetRotation();
		glm::vec3 tfScale = tf->GetScale();

		ImGui::Text("Transform Component [%p]", tf);
		
		if (ImGui::DragFloat3("Position", glm::value_ptr(tfPos))) {
			if (tfPos != tf->GetPosition()) {
				static_cast<Transform*>(this)->SetPosition(tfPos);
			}
		}

		if (ImGui::DragFloat3("Rotation", glm::value_ptr(tfRot))) {
			static_cast<Transform*>(this)->SetRotation(tfRot);
		}

		if (ImGui::DragFloat3("Scale", glm::value_ptr(tfScale))) {
			static_cast<Transform*>(this)->SetScale(tfScale);
		}



	}

	void Transform::Save(nlohmann::json& j)
	{

		Debug::Log(DEBUG, "Saving Transform %p...", this);

		// Save transform data
		j["transform"]["position"] = ng::Assets::Vec3ToJson(position);
		j["transform"]["rotation"] = ng::Assets::Vec3ToJson(rotation);
		j["transform"]["scale"] = ng::Assets::Vec3ToJson(scale);

		Debug::Log(DEBUG, "Finished saving Transform %p", this);

	}

	void Transform::Load(const nlohmann::json& j)
	{

		if (!j.contains("transform")) return;
		const nlohmann::json& t = j.at("transform");

		SetPosition(ng::Assets::ReadVec3(t, "position", position));
		SetRotation(ng::Assets::ReadVec3(t, "rotation", rotation));
		SetScale(ng::Assets::ReadVec3(t, "scale", scale));

	}
