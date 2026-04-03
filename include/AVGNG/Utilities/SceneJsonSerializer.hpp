#pragma once

#include "AVGNG/Core/Scene.hpp"

namespace ng::Assets {

	class SceneJsonSerializer {

	public:
		static void SerializeSceneToJson(ng::Core::Scene& scene, const char* filepath); // save
		static void DeserializeSceneFromJson(const char* filePath); // load

	};


}