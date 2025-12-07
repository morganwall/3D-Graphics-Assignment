#include "ProjectWindow.h"

void ProjectWindow::Draw()
{
	if (!active)
		return;

	ImGui::Begin("Project", nullptr, ImGuiWindowFlags_NoCollapse);

	ImGui::End();
}

ProjectWindow::ProjectWindow(bool isActive) : Window(isActive)
{

}

ProjectWindow::~ProjectWindow()
{

}