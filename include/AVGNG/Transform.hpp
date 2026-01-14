#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <AVGNG/Component.hpp>

namespace ng::Core {


	class Transform : public Component {

		glm::vec3 position{ 0.0f };
		glm::vec3 rotation{ 0.0f }; // in radians (pitch, yaw, roll)
		glm::vec3 scale{ 1.0f };

	public:
		glm::mat4 GetModelMatrix() const
		{

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, position);
			model = glm::rotate(model, rotation.x, glm::vec3(1, 0, 0));
			model = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));
			model = glm::rotate(model, rotation.z, glm::vec3(0, 0, 1));

			model = glm::scale(model, scale);

			return model;

		}
		
		void SetPosition(glm::vec3 pos);
		void SetRotation(glm::vec3 rot);
		void SetScale(glm::vec3 scale);

		glm::vec3 GetPosition() const { return position; }
		glm::vec3 GetRotation() const { return rotation; }
		glm::vec3 GetScale() const { return scale; }

		void OnInspectorGUI() override;

	
	};

}
