#pragma once

class EditorUIElement {
public:
	virtual void Show() = 0;
	virtual void Hide() = 0;
	virtual bool IsVisible() = 0;
};