#pragma once
#include "Window.h"
#include "../Objects/SceneObject.h"

class InspectorWindow : public Window
{
public:
	void Draw(SceneObject* selectedObject);

	InspectorWindow(bool isActive = true);
	~InspectorWindow();
};