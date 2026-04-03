#include "AVGNG/Core/Transform.hpp"
#include "AVGNG/Utilities/Debug.hpp"
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
		
		if (ImGui::InputFloat3("Position", glm::value_ptr(tfPos))) {
			if (tfPos != tf->GetPosition()) {
				static_cast<Transform*>(this)->SetPosition(tfPos);
			}
		}

		if (ImGui::InputFloat3("Rotation", glm::value_ptr(tfRot))) {
			static_cast<Transform*>(this)->SetRotation(tfRot);
		}

		if (ImGui::InputFloat3("Scale", glm::value_ptr(tfScale))) {
			static_cast<Transform*>(this)->SetScale(tfScale);
		}



	}

	void Transform::Save(nlohmann::json& j, int componentIndex)
	{

		glm::vec3 pos = this->GetPosition();
		glm::vec3 rot = this->GetRotation();
		glm::vec3 scl = this->GetScale();

		Debug::Log(DEBUG, "Saving Transform %p...", this);

		//j = nlohmann::json::array();

		// Save transform data
		j["transform"]["position"] = { pos.x, pos.y, pos.z };
		j["transform"]["rotation"] = { rot.x, rot.y, rot.z };
		j["transform"]["scale"] = { scl.x, scl.y, scl.z };

		Debug::Log(DEBUG, "Finished saving Transform %p", this);

	}
