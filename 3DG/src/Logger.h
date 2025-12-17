#pragma once
#include <vector>
#include <string>
#include "EngineStructs.h"
#include "GUI/ConsoleWindow.h"

// Used to Log to the Console Window in the GUI.
class Logger
{
	ConsoleWindow* consoleWindow{ nullptr };
	std::vector<Engine::LoggerMsg*>* logBuffer; // Console window's log buffer.

public:
	void Add(Engine::LoggerMsg* msg);
	void Add(std::vector<Engine::LoggerMsg*> msgs);

	Logger(ConsoleWindow* consoleWindow);
	~Logger();
};