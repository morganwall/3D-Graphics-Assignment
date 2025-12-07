#include "ConsoleWindow.h"

void ConsoleWindow::Draw()
{
	if (!active)
		return;

	ImGui::Begin("Console", nullptr, ImGuiWindowFlags_NoCollapse);

	ImGui::End();
}

ConsoleWindow::ConsoleWindow(bool isActive) : Window(isActive)
{

}

ConsoleWindow::~ConsoleWindow()
{

}