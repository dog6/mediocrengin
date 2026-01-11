#include <AVGNG/Transform.hpp>

using namespace ng::Core;


	void Transform::SetPosition(glm::vec3 pos) {
		this->position = pos;
	}

	void Transform::SetRotation(glm::vec3 rot) {
		this->rotation = rot;
	}

	void Transform::SetScale(glm::vec3 scale) {
		this->scale = scale;
	}

