#include "AVGNG/Core/Transform.hpp"
#include "AVGNG/Core/Debug.hpp"
#include "AVGNG/Assets/JsonUtils.hpp"
#include <imgui/imgui.h>
#include "AVGNG/Core/GameObject.hpp"

using namespace ng::Core;


Transform::Transform()
{
	this->SetPosition(glm::vec3(0));
	this->SetRotation(glm::vec3(0));
	this->SetScale(glm::vec3(1));
}

Transform::~Transform()
{
	// The children lose their parent.
	for (Transform* child : children) {
		child->parent = nullptr;
	}
	children.clear();

	// This object leaves the child list of its parent.
	ClearParent();
}

glm::mat4 Transform::GetWorldMatrix() const
{
	const glm::mat4 local = GetModelMatrix();

	// With a parent: the position is local to the parent.
	if (parent != nullptr) {
		return parent->GetWorldMatrix() * local;
	}

	// Without a parent: the position is in world space.
	return local;
}

void Transform::SetPosition(glm::vec3 pos)
{
    this->position = pos;
}

    void Transform::SetRotation(glm::vec3 rot) {
		this->rotation = rot;
	}

	void Transform::SetScale(glm::vec3 scale) {
		this->scale = scale;
	}


	void Transform::OnInspectorGUI() {

	// Drag speeds. Change these values to make the drag faster or slower.
	constexpr float POSITION_SPEED = 0.01f;
	constexpr float ROTATION_SPEED = 0.5f;
	constexpr float SCALE_SPEED    = 0.01f;

	ImGui::Text("Transform Component [%p]", (void*)this);

	// Copy the values. The drag control changes the copy.
	glm::vec3 pos   = GetPosition();
	glm::vec3 rot   = GetRotation();
	glm::vec3 scale = GetScale();

	// DragFloat3 gives back "true" only when the value changes.
	if (ImGui::DragFloat3("Position", glm::value_ptr(pos), POSITION_SPEED, 0.0f, 0.0f, "%.3f")) {
		SetPosition(pos);
	}

	if (ImGui::DragFloat3("Rotation", glm::value_ptr(rot), ROTATION_SPEED, 0.0f, 0.0f, "%.2f")) {
		SetRotation(rot);
	}

	// The minimum value of 0.001 prevents a scale of 0.
	if (ImGui::DragFloat3("Scale", glm::value_ptr(scale), SCALE_SPEED, 0.001f, FLT_MAX, "%.3f")) {
		SetScale(scale);
	}

	// ---- Hierarchy ----
	ImGui::Separator();
	ImGui::Text("Hierarchy");

	Transform* currentParent = GetParent();

	if (currentParent != nullptr) {
		ImGui::Text("Parent: Transform [%p]", (void*)currentParent);

		// The values above are local. Show the world values as read-only text.
		const glm::vec3 worldPos   = GetWorldPosition();
		const glm::vec3 worldScale = GetWorldScale();
		ImGui::Text("World Position: %.3f, %.3f, %.3f", worldPos.x, worldPos.y, worldPos.z);
		ImGui::Text("World Scale: %.3f, %.3f, %.3f", worldScale.x, worldScale.y, worldScale.z);

		if (ImGui::Button("Clear Parent")) {
			ClearParent();
		}
	}
	else {
		ImGui::TextDisabled("Parent: None");
	}

	ImGui::Text("Children: %zu", GetChildCount());
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
