#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <nlohmann/json.hpp>
#include "AVGNG/Core/IComponent.hpp"

namespace ng::Core {


	class Transform : public IComponent {

		glm::vec3 position{ 0.0f };
		glm::vec3 rotation{ 0.0f }; // in radians (pitch, yaw, roll)
		glm::vec3 scale{ 1.0f };
		Transform* parent = nullptr;
        std::vector<Transform*> children;

	void EraseChildEntry(Transform* child) {
		auto it = std::find(children.begin(), children.end(), child);
		if (it != children.end()) {
			children.erase(it);
		}
	}

	public:
		Transform();
		~Transform();

	void SetParent(Transform* newParent) {
		if (newParent == parent) {
			return;
		}

		// Reject a cycle. This also rejects "newParent == this".
		for (Transform* t = newParent; t != nullptr; t = t->parent) {
			if (t == this) {
				return;
			}
		}

		// Leave the child list of the old parent.
		if (parent != nullptr) {
			parent->EraseChildEntry(this);
		}

		parent = newParent;

		// Join the child list of the new parent.
		if (parent != nullptr) {
			parent->children.push_back(this);
		}
	}

	Transform* GetParent() const { return parent; }

	void ClearParent() { SetParent(nullptr); }

	void AddChild(Transform* child) {
		if (child != nullptr) {
			child->SetParent(this);
		}
	}

	void RemoveChild(Transform* child) {
		if (child != nullptr && child->parent == this) {
			child->SetParent(nullptr);
		}
	}

	size_t GetChildCount() const { return children.size(); }

	const std::vector<Transform*>& GetChildren() const { return children; }

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
		
		glm::mat4 GetWorldMatrix() const;

		glm::vec3 GetWorldPosition() const {
			glm::mat4 world = GetWorldMatrix();
			return glm::vec3(world[3]);
		}

		glm::vec3 GetWorldScale() const {
			glm::mat4 world = GetWorldMatrix();
			glm::vec3 worldScale;
			worldScale.x = glm::length(glm::vec3(world[0]));
			worldScale.y = glm::length(glm::vec3(world[1]));
			worldScale.z = glm::length(glm::vec3(world[2]));
			return worldScale;
		}

		void SetPosition(glm::vec3 pos);
		void SetRotation(glm::vec3 rot);
		void SetScale(glm::vec3 scale);

		glm::vec3 GetPosition() const { return position; }
		glm::vec3 GetRotation() const { return rotation; }
		glm::vec3 GetScale() const { return scale; }

		void OnInspectorGUI() override;

		void Save(nlohmann::json& j) override;
		void Load(const nlohmann::json& j) override;

	};

}
