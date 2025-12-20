#include "ConsoleWindow.h"
#include <iostream>

void ConsoleWindow::Draw()
{
	if (!active)
		return;

	ImGui::Begin("Console", nullptr, ImGuiWindowFlags_NoCollapse);

	// Loop through Logger Messages.
	for (size_t i = 0; i < logBuffer.size(); )
	{
		Engine::LoggerMsg* curMsg = logBuffer[i];
		std::string msgBuffer;

		// Switch on Log Type. TODO: Move this into a function.
		switch (curMsg->type)
		{
		case Engine::LogType::INFO:
			msgBuffer = "[.] " + curMsg->timestamp + " | " + curMsg->name + " | " + curMsg->description;
			curMsg->colour = { 1.0f, 1.0f, 1.0f, 1.0f };
			break;
		case Engine::LogType::ERROR:
			msgBuffer = "[!] " + curMsg->timestamp + " | " + curMsg->name + " | " + curMsg->description;
			curMsg->colour = { 1.0f, 0.0f, 0.0f, 1.0f };
			break;
		case Engine::LogType::SUCCESS:
			msgBuffer = "[+] " + curMsg->timestamp + " | " + curMsg->name + " | " + curMsg->description;
			curMsg->colour = { 0.0f, 1.0f, 0.0f, 1.0f };
			break;
		case Engine::LogType::CUSTOM:
		default:
			msgBuffer = "[ ] " + curMsg->timestamp + " | " + curMsg->name + " | " + curMsg->description;
			break;
		}

		// Move GUI Messages to a Separate Vector, as they Can't be Removed.
		if (curMsg->toGUI)
		{
			guiMessages.push_back(curMsg);
			curMsg->toGUI = false;
		}

		// Output Console Message.
		if (curMsg->toConsole)
		{
			std::cout << msgBuffer << "\n";
			curMsg->toConsole = false;
		}

		// Check if the Message has Already Been Output, and or Moved to the GUI Message Vector.
		if (!curMsg->toGUI && !curMsg->toConsole)
		{
			logBuffer.erase(logBuffer.begin() + i);
			curMsg = nullptr;
		}
		else
		{
			// Increment if Vector Hasn't Been Changed.
			i++;
		}
	}

	// Loop through GUI Messages.
	for (Engine::LoggerMsg* curMsg : guiMessages)
	{
		std::string msgBuffer;

		// Switch on Log Type.
		switch (curMsg->type)
		{
		case Engine::LogType::INFO:
			msgBuffer = "[.] " + curMsg->timestamp + " | " + curMsg->name + " | " + curMsg->description;
			curMsg->colour = { 1.0f, 1.0f, 1.0f, 1.0f };
			break;
		case Engine::LogType::ERROR:
			msgBuffer = "[!] " + curMsg->timestamp + " | " + curMsg->name + " | " + curMsg->description;
			curMsg->colour = { 1.0f, 0.0f, 0.0f, 1.0f };
			break;
		case Engine::LogType::SUCCESS:
			msgBuffer = "[+] " + curMsg->timestamp + " | " + curMsg->name + " | " + curMsg->description;
			curMsg->colour = { 0.0f, 1.0f, 0.0f, 1.0f };
			break;
		case Engine::LogType::CUSTOM:
		default:
			msgBuffer = "[ ] " + curMsg->timestamp + " | " + curMsg->name + " | " + curMsg->description;
			break;
		}

		ImGui::TextColored(curMsg->colour, msgBuffer.c_str());
	}

	ImGui::End();
}

ConsoleWindow::ConsoleWindow(bool isActive) : Window(isActive)
{

}

ConsoleWindow::~ConsoleWindow()
{

}