#pragma once
#include <nlohmann/json.hpp>

namespace ng::Assets {

	class IJsonSerializable {

	public:
		virtual void Save(nlohmann::json& j, int componentIndex) = 0;

	};

};