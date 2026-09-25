#include "AVGNG/Core/collision/BoxShape.hpp"
#include <imgui/imgui.h>

using namespace ng::Core;

void BoxShape::OnInspectorGUI()
{
    ImGui::DragFloat3("Half Extents", &halfExtents.x, 0.01f, 0.001f, 1000.0f);
}