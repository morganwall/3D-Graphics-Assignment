#include "InspectorWindow.h"

void InspectorWindow::Draw(SceneObject* selectedObject)
{
	if (!active)
		return;

	ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoCollapse);

	if (selectedObject)
		selectedObject->GUI();

	ImGui::End();
}

InspectorWindow::InspectorWindow(bool isActive) : Window(isActive)
{

}

InspectorWindow::~InspectorWindow()
{

}