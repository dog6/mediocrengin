#pragma once

#include "AVGNG/Core/Scene.hpp"
#include <string_view>

namespace ng::Assets {

    class SceneJsonSerializer {
    public:
        static bool SerializeSceneToJson(ng::Core::Scene& scene, std::string_view filepath);
        static ng::Core::Scene* DeserializeSceneFromJson(std::string_view filePath, ng::Graphics::Camera* camera, glm::uvec2& viewportSize);
    };

}