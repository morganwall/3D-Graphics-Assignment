#include "ProjectWindow.h"

void ProjectWindow::Draw()
{
	if (!active)
		return;

	ImGui::Begin("Project", nullptr, ImGuiWindowFlags_NoCollapse);

    // Get ImGui Window Size.
    ImVec2 wndSize{ ImGui::GetContentRegionAvail() };

    ImGui::BeginChild("Left", { wndSize.x / 5.0f, wndSize.y }, true);
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("Right", { 0.0f, wndSize.y }, true);
    ImGui::EndChild();

	ImGui::End();
}

ProjectWindow::ProjectWindow(bool isActive) : Window(isActive)
{

}

ProjectWindow::~ProjectWindow()
{

}