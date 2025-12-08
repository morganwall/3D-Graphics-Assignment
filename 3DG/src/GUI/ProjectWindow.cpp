#include "ProjectWindow.h"

void ProjectWindow::Draw()
{
	if (!active)
		return;

	ImGui::Begin("Project", nullptr, ImGuiWindowFlags_NoCollapse);

    // Get ImGui Window Size.
    ImVec2 wndSize{ ImGui::GetContentRegionAvail() };

    ImGui::BeginChild("Left", { wndSize.x / 5.0f, wndSize.y }, true);
	ImGui::Text("..");

	// Check if the Text Element is clicked.
	if (ImGui::IsItemClicked())
		if (currentPath.has_parent_path()) // Check if the directory has a parent path.
			currentPath = currentPath.parent_path(); // Go to parent directory.

	// Loop Through all Files and Directories within currentPath.
	for (const auto& entry : std::filesystem::directory_iterator(currentPath))
	{
		// Get the Current File Name.
		std::string filename = entry.path().filename().string();

		// Create ImGui Text Element for the File or Directory.
		ImGui::Text(filename.c_str());

		// Check if the Text Element is clicked.
		if (ImGui::IsItemClicked())
		{
			// Check if the Text Element is a Directory or a File.
			if (entry.is_directory())
			{
				currentPath /= filename;
			}

			// TODO: Show File Contents on Click.
		}
	}
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