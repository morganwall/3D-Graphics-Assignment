#pragma once
#include "Window.h"
#include <vector>
#include <string>

class ConsoleWindow : public Window
{
	std::vector<std::string> logLines;

public:
	virtual void Draw() override;

	ConsoleWindow(bool isActive = true);
	~ConsoleWindow();
};