#pragma once
#include "Window.h"
#include <string>
#include <filesystem>
#include <vector>
#include <iostream>

class ProjectWindow : public Window
{
	std::filesystem::path currentPath{ "Data" };
	int browserNumColumns{ 4 }; // The max number of columns in the file browser.

public:
	virtual void Draw() override;

	ProjectWindow(bool isActive = true);
	~ProjectWindow();
};