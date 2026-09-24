#pragma once

#include "AVGNG/Core/Scene.hpp"

namespace ng::Assets {

	class SceneJsonSerializer {

	public:
		static void SerializeSceneToJson(ng::Core::Scene& scene, const char* filepath); // save
		static ng::Core::Scene* DeserializeSceneFromJson(const char* filePath, ng::Graphics::Camera* camera, glm::uvec2& viewportSize); // load, returns nullptr on failure

	};


}