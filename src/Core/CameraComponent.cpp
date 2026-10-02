#include "AVGNG/Core/CameraComponent.hpp"
#include "AVGNG/Core/GameObject.hpp"
#include <imgui/imgui.h>

using namespace ng::Core;

CameraComponent::CameraComponent() {}

CameraComponent::~CameraComponent() {}

void CameraComponent::Apply()
{
	if (s_mainCamera == nullptr) return;

	// Cache the transform if the pointer is null.
	// "owner" is not set yet in the constructor.
	if (tf == nullptr) {
		if (owner != nullptr) {
			tf = owner->GetComponent<Transform>();
		}
		if (tf == nullptr) return;
	}

	// The world matrix includes all parents. This is the same matrix the mesh uses.
	const glm::mat4 world = tf->GetWorldMatrix();

	const glm::vec3 position = glm::vec3(world[3]);

	// The camera looks along -Z in local space. Up is +Y in local space.
	// w = 0 gives a direction. The translation part does not apply.
	const glm::vec3 forward = glm::normalize(glm::vec3(world * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
	const glm::vec3 up      = glm::normalize(glm::vec3(world * glm::vec4(0.0f, 1.0f,  0.0f, 0.0f)));

	s_mainCamera->SetPosition(position);
	s_mainCamera->SetTarget(position + forward);
	s_mainCamera->SetUp(up);
}

void CameraComponent::Update(float deltaTime)
{
	(void)deltaTime;
	Apply();
}

void CameraComponent::OnInspectorGUI()
{
	ImGui::PushID(this);

	ImGui::Text("CameraComponent [%p]", (void*)this);

	if (s_mainCamera == nullptr) {
		ImGui::TextDisabled("No main camera set. Call CameraComponent::SetMainCamera().");
	}
	else {
		const glm::vec3& p = s_mainCamera->GetPosition();
		ImGui::Text("Camera Position: %.3f, %.3f, %.3f", p.x, p.y, p.z);
	}

	ImGui::PopID();
}

void CameraComponent::Save(nlohmann::json& j)
{
	// This component has no data. The key marks that the component exists.
	j["cameraComponent"] = nlohmann::json::object();
}

void CameraComponent::Load(const nlohmann::json& j)
{
	(void)j;
}