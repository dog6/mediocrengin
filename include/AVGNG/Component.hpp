#pragma once



namespace ng::Core {

	class GameObject;

		class Component {

		public:
			ng::Core::GameObject* owner = nullptr;
			virtual ~Component() = default;

			virtual void Start() {}
			virtual void Update(float detltaTime) {}
			virtual void OnInspectorGUI() = 0;
		};

}