#pragma once

#include "Transform.hpp"
#include "Mesh.hpp"

using namespace ng::Graphics;

namespace ng {
namespace Core {


	class GameObject {

	public:
		Transform transform;
		Mesh* mesh;

		void SetPosition(glm::vec3 pos) {
			this->transform.position = pos;
		}

		void SetRotation(glm::vec3 rot) {
			this->transform.rotation = rot;
		}

		void SetScale(glm::vec3 scale) {
			this->transform.scale = scale;
		}

	};

}}
