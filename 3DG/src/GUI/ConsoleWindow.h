#pragma once
#include "Window.h"
#include <vector>
#include <string>
#include "../EngineStructs.h"

class ConsoleWindow : public Window
{
	std::vector<Engine::LoggerMsg*> logBuffer; // Holds messages to be added to the log.
	std::vector<Engine::LoggerMsg*> guiMessages; // Holds GUI messages.

public:
    std::vector<Engine::LoggerMsg*>* GetLogBuffer() { return &logBuffer; }

	virtual void Draw() override;

	ConsoleWindow(bool isActive = true);
	~ConsoleWindow();
};