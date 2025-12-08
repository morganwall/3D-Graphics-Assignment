#pragma once
#include "Window.h"
#include <string>
#include <filesystem>
#include <vector>
#include <iostream>

class ProjectWindow : public Window
{
	std::filesystem::path currentPath{ "Data/" };


public:
	virtual void Draw() override;

	ProjectWindow(bool isActive = true);
	~ProjectWindow();
};