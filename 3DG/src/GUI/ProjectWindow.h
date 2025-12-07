#pragma once
#include "Window.h"

class ProjectWindow : public Window
{
public:
	virtual void Draw() override;

	ProjectWindow(bool isActive = true);
	~ProjectWindow();
};