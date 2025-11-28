#pragma once
#include <vector>
#include <glew.h>
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Objects/SceneObject.h"
#include "Objects/Cube.h"
#include "Objects/Terrain.h"
#include "Objects/Skybox.h"
#include "Objects/Lights/Light.h"
#include "Objects/Lights/DirectionalLight.h"
#include "Objects/Lights/PointLight.h"
#include "Objects/Lights/SpotLight.h"

class GUI
{
	std::vector<SceneObject*>& sceneObjects;
	SceneObject* selectedObject{ nullptr };

	bool showObjectsWindow{ true };
	bool showInspectorWindow{ true };
	bool showWorldSettingsWindow{ true };

public:
	// Getters.
	SceneObject* GetSelectedObject() { return selectedObject; }
	bool GetShowObjectsWindow() { return showObjectsWindow; }
	bool GetShowInspectorWindow() { return showInspectorWindow; }
	bool GetShowWorldSettingsWindow() { return showWorldSettingsWindow; }

	// Setters.
	void SetSelectedObject(SceneObject* object) { selectedObject = object; }
	void SetShowObjectsWindow(bool visibility) { showObjectsWindow = visibility; }
	void SetShowInspectorWindow(bool visibility) { showInspectorWindow = visibility; }
	void SetShowWorldSettingsWindow(bool visibility) { showWorldSettingsWindow = visibility; }

	void DeleteObject(SceneObject* target);

	void Viewport(GLFWwindow* window);
	void ObjectsWindow();
	void InspectorWindow();
	void WorldSettingsWindow(glm::vec4& clearColour);

	void DrawGUI(GLFWwindow* window, glm::vec4& clearColour);

	GUI(std::vector<SceneObject*>& sceneObjectVector) : sceneObjects(sceneObjectVector) {};
	~GUI() {};
};