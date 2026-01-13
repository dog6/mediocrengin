#pragma once

namespace ng::Editor {

	class InspectorView {

	private:
		static bool s_isVisible;
		static void CreateUI(); // updates UI elements

	public:

		static void Show() { s_isVisible = true; } // shows UI
		static void Hide() { s_isVisible = false; } // hides UI
		static bool IsVisible() { return s_isVisible; }
		
		static void Update() { CreateUI(); }


	};

}