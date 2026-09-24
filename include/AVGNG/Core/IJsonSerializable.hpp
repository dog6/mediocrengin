#pragma once
#include <nlohmann/json.hpp>

namespace ng::Assets {

	class IJsonSerializable {

	public:
		virtual ~IJsonSerializable() = default;

		// Writes this object under its own type key, e.g. j["transform"] = {...}
		virtual void Save(nlohmann::json& j) = 0;

		// Reads this object back from the same shape Save produced; j is the
		// object containing the type key. Missing fields keep current values.
		virtual void Load(const nlohmann::json& j) = 0;

	};

};
