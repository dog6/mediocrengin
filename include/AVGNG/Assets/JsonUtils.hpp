#pragma once

#include <nlohmann/json.hpp>
#include <glm/glm.hpp>

namespace ng::Assets {

	// Reads a [x, y, z] JSON array at parent[key], falling back if the value
	// is missing or malformed. Never inserts into parent (unlike operator[]).
	inline glm::vec3 ReadVec3(const nlohmann::json& parent, const char* key, glm::vec3 fallback = glm::vec3(0.0f))
	{
		auto it = parent.find(key);
		if (it == parent.end() || !it->is_array() || it->size() < 3) return fallback;

		const nlohmann::json& arr = *it;
		if (!arr[0].is_number() || !arr[1].is_number() || !arr[2].is_number()) return fallback;

		return glm::vec3(arr[0].get<float>(), arr[1].get<float>(), arr[2].get<float>());
	}

	// Reads a numeric JSON value at parent[key], falling back if the value
	// is missing or not a number. Never inserts into parent (unlike operator[]).
	inline float ReadFloat(const nlohmann::json& parent, const char* key, float fallback = 0.0f)
	{
		auto it = parent.find(key);
		if (it == parent.end() || !it->is_number()) return fallback;

		return it->get<float>();
	}

	inline nlohmann::json Vec3ToJson(const glm::vec3& v)
	{
		return { v.x, v.y, v.z };
	}

}
