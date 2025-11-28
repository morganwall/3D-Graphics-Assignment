#include "GUI.h"

extern Light* light;

void GUI::DeleteObject(SceneObject* target)
{
	// Stop Camera Deletion.
	if (!target || target->GetObjectType() == SceneObjectType::CAMERA)
		return;

	// Check if we're Deleting the Light.
	if (target == light)
		light = nullptr;

	// Reset Selection if this Object is Currently Selected.
	if (selectedObject == target)
		selectedObject = nullptr;

	// Find the object in the Scene Objects Vector.
	auto inVec{ std::find(sceneObjects.begin(), sceneObjects.end(), target) };
	if (inVec != sceneObjects.end()) // Check if we found the selected object in the scene objects vector.
	{
		// Cleanup Object.
		target->Cleanup();

		// Remove Object from Scene Objects Vector.
		sceneObjects.erase(inVec);

		// Delete Object.
		delete target;
	}
}

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
			ImGui::MenuItem("Objects", "F1", &showObjectsWindow);
			ImGui::MenuItem("Inspector", "F2", &showInspectorWindow);
			ImGui::MenuItem("World Settings", "F3", &showWorldSettingsWindow);

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

void GUI::ObjectsWindow()
{
	if (showObjectsWindow)
	{
		ImGui::Begin("Objects", nullptr, ImGuiWindowFlags_NoCollapse);

		ImGui::Text("Scene Objects");
		ImVec2 newObjectButtonSize{ 50.0f, 20.0f };
		ImGui::SameLine(ImGui::GetWindowWidth() - newObjectButtonSize.x - 20.0f);
		if (ImGui::Button("+", newObjectButtonSize))
			ImGui::OpenPopup("NewObjectPopup");

		// Popup for Creating a New Object.
		if (ImGui::BeginPopup("NewObjectPopup"))
		{
			ImGui::Text("New Scene Object");

			ImGui::Separator();

			if (ImGui::MenuItem("Cube"))
				sceneObjects.push_back(new Cube());
			if (ImGui::MenuItem("Terrain"))
				sceneObjects.push_back(new Terrain());
			if (ImGui::MenuItem("Directional Light"))
				sceneObjects.push_back(new DirectionalLight());
			if (ImGui::MenuItem("Point Light"))
				sceneObjects.push_back(new PointLight());
			if (ImGui::MenuItem("Spot Light"))
				sceneObjects.push_back(new SpotLight());
			if (ImGui::MenuItem("Skybox"))
				sceneObjects.push_back(new Skybox());

			ImGui::EndPopup();
		}

		ImGui::Separator();

		ImGui::BeginChild("Objects", { 0, 0 }, true);
		
		unsigned int id{ 0 }; // ID for ImGui elements.

		SceneObject* toDelete{ nullptr }; // Used to store a Scene Object that should be deleted.

		// Loop through Scene Objects.
		for (auto& curObject : sceneObjects)
		{
			bool isSelected{ false };
			if (curObject == selectedObject)
				isSelected = true;
			else
				isSelected = false;

			// Push Object Pointer as ImGui ID.
			ImGui::PushID(curObject);

			// Create Object Selectable.
			if (ImGui::Selectable(curObject->GetName().c_str(), isSelected))
				selectedObject = curObject;

			// Change Background Colour on Selection.
			if (isSelected)
				ImGui::SetItemDefaultFocus();

			// Right Click Menu.
			if (curObject->GetObjectType() != SceneObjectType::CAMERA) // Make sure we're not right clicking on a Camera object. I don't want the camera to be deleteable.
			{
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Delete"))
						toDelete = curObject;

					ImGui::EndPopup();
				}
			}

			ImGui::PopID();
		}

		ImGui::EndChild();

		// Delete Object.
		if (toDelete)
			DeleteObject(toDelete);

		ImGui::End();
	}
}

void GUI::InspectorWindow()
{
	if (showInspectorWindow)
	{
		ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoCollapse);

		if (selectedObject)
			selectedObject->GUI();

		ImGui::End();
	}
}

void GUI::WorldSettingsWindow(glm::vec4& clearColour)
{
	if (showWorldSettingsWindow)
	{
		ImGui::Begin("World Settings", nullptr, ImGuiWindowFlags_NoCollapse);

		ImGui::Text("FPS: %.0f", ImGui::GetIO().Framerate);
		ImGui::Text("Frame Time: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);

		ImGui::Separator();

		ImGui::PushItemWidth(150.0f);
		ImGui::ColorPicker3("Clear Colour", &clearColour.x, ImGuiColorEditFlags_DisplayRGB);
		ImGui::PopItemWidth();

		ImGui::End();
	}
}

void GUI::DrawGUI(GLFWwindow* window, glm::vec4& clearColour)
{
	Viewport(window);
	InspectorWindow();
	WorldSettingsWindow(clearColour);
	ObjectsWindow();

	if (showImGuiDemo)
		ImGui::ShowDemoWindow();
}