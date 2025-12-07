#pragma once
#include "imgui.h"

class Window
{
protected:
	bool active{ true };

public:
	// Getters.
	bool GetActive() { return active; }

	// Setters.
	void SetActive(bool isActive) { active = isActive; }

	virtual void Draw();

	Window(bool isActive = true);
	~Window();
};