#pragma once
#include <vector>
#include <glew.h>
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ObjectsWindow.h"
#include "ProjectWindow.h"
#include "InspectorWindow.h"
#include "WorldSettingsWindow.h"
#include "ConsoleWindow.h"

class GUI
{
	ObjectsWindow* objectsWindow{ nullptr };
	InspectorWindow* inspectorWindow{ nullptr };
	WorldSettingsWindow* worldSettingsWindow{ nullptr };
	ProjectWindow* projectWindow{ nullptr };
	ConsoleWindow* consoleWindow{ nullptr };

	bool showImGuiDemo{ false };

public:
	// Getters.
	ObjectsWindow* GetObjectsWindow() { return objectsWindow; }
	InspectorWindow* GetInspectorWindow() { return inspectorWindow; }
	WorldSettingsWindow* GetWorldSettingsWindow() { return worldSettingsWindow; }
	ProjectWindow* GetProjectWindow() { return projectWindow; }
	ConsoleWindow* GetConsoleWindow() { return consoleWindow; }

	void Viewport(GLFWwindow* window);
	void DrawGUI(GLFWwindow* window, glm::vec4& clearColour, float& ambientIntensity);

	GUI(std::vector<SceneObject*>& sceneObjectVector);
	~GUI() {};
};