#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <sol/sol.hpp>

#include "AVGNG/Assets/AssimpObjLoader.hpp"

namespace ng::Core {

	enum CursorLockMode {

		NONE = 0,
		LOCKED = 1,
		CONFINED = 2

	};

	struct CursorLockState {

		public:
			bool isVisible = true;
			CursorLockMode lockMode;

	};

	class Cursor {

	private:
		static CursorLockState s_lockState;
		static GLFWwindow* s_gameWindow;

	public:


		// Getters/Setters
		static  void SetCursorLockMode(CursorLockMode mode); //{
		// 	Debug::Log(DEBUG, "Set cursor lock mode to: %d", (int)mode);
		// 	s_lockState.lockMode = mode;
		// 	OnCursorLockModeChanged();
		// }
		static CursorLockMode GetCursorLockMode(); //{
		// 	return s_lockState.lockMode;
		// }

		// Methods
		static void Init(GLFWwindow* gameWindow);
		static void OnCursorLockModeChanged();
		static void RegisterCursorWithLua(sol::state& lua);

	};

}