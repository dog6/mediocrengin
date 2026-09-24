#include "SphereCollider.hpp"
#include <imgui.h>

using namespace ng::Core;

ng::Core::SphereCollider::SphereCollider(){}

ng::Core::SphereCollider::~SphereCollider(){}

void ng::Core::SphereCollider::OnInspectorGUI()
{

    ImGui::Text("SphereCollider Component [%p]", this);

    if (ImGui::InputFloat("Radius", &radius)) {
        static_cast<SphereCollider*>(this)->SetRadius(radius);
    }

}
