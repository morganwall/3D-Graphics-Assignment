#include "Logger.h"
#include <iostream>
#include "GUI/ConsoleWindow.h"

void Logger::Add(Engine::LoggerMsg* msg)
{
	logBuffer->push_back(msg);
}

void Logger::Add(std::vector<Engine::LoggerMsg*> msgs)
{
	// Loop through All Messages.
	for (Engine::LoggerMsg* curMsg : msgs)
		Add(curMsg);
}

Logger::Logger(ConsoleWindow* consoleWindow) : consoleWindow(consoleWindow)
{
	logBuffer = consoleWindow->GetLogBuffer();
}

Logger::~Logger()
{

}