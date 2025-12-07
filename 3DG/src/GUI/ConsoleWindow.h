#pragma once
#include "Window.h"

class ConsoleWindow : public Window
{
public:
	virtual void Draw() override;

	ConsoleWindow(bool isActive = true);
	~ConsoleWindow();
};