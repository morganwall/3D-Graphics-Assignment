#include "GUI.h"

void GUI::Viewport(GLFWwindow* window)
{
	ImGuiViewport* viewport{ ImGui::GetMainViewport() };
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGuiWindowFlags wndFlags
	{
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_MenuBar |
		ImGuiWindowFlags_NoNavFocus |
		ImGuiWindowFlags_NoBringToFrontOnFocus
	};

	// Push Style for Dockspace.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	// Create Dockspace.
	ImGui::Begin("DockSpace", nullptr, wndFlags);

	// Pop Dockspace Style.
	ImGui::PopStyleVar(3); // 3 is the number of style vars we pushed.

	unsigned int dockSpaceID{ ImGui::GetID("DockingSpaceMain") };
	ImGui::DockSpace(dockSpaceID, { 0.0f, 0.0f }, ImGuiDockNodeFlags_PassthruCentralNode); // ImGuiDockNodeFlags_PassthruCentralNode makes the viewport background see through.

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Windows"))
		{
			ImGui::MenuItem("Objects", "F1", objectsWindow->GetActive());
			ImGui::MenuItem("Inspector", "F2", inspectorWindow->GetActive());
			ImGui::MenuItem("World Settings", "F3", worldSettingsWindow->GetActive());
			ImGui::MenuItem("Project", "F4", projectWindow->GetActive());
			ImGui::MenuItem("Console", "F5", consoleWindow->GetActive());

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Tools"))
		{
			ImGui::MenuItem("ImGui Demo", nullptr, &showImGuiDemo);

			ImGui::EndMenu();
		}

		ImGui::SameLine(ImGui::GetContentRegionAvail().x + ImGui::CalcTextSize("Exit").x + ImGui::GetStyle().FramePadding.x + 40.0f);
		if (ImGui::MenuItem("Exit", nullptr, nullptr))
			glfwSetWindowShouldClose(window, GLFW_TRUE);

		ImGui::EndMenuBar();
	}

	ImGui::End();
}

void GUI::DrawGUI(GLFWwindow* window, glm::vec4& clearColour, float& ambientIntensity)
{
	Viewport(window);

	// Draw Windows.
	inspectorWindow->Draw(objectsWindow->GetSelectedObject());
	worldSettingsWindow->Draw(clearColour, ambientIntensity);
	objectsWindow->Draw();
	consoleWindow->Draw();
	projectWindow->Draw();

	if (showImGuiDemo)
		ImGui::ShowDemoWindow();
}

GUI::GUI(std::vector<SceneObject*>& sceneObjectVector)
{
	// Create Window Objects.
	inspectorWindow = new InspectorWindow();
	worldSettingsWindow = new WorldSettingsWindow();
	objectsWindow = new ObjectsWindow(true, &sceneObjectVector);
	consoleWindow = new ConsoleWindow();
	projectWindow = new ProjectWindow();
}