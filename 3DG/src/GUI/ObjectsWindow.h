#pragma once
#include "Window.h"
#include <vector>
#include "../Objects/SceneObject.h"

class ObjectsWindow : public Window
{
	std::vector<SceneObject*>* sceneObjects;
	SceneObject* selectedObject{ nullptr };

public:
	// Getters.
	SceneObject* GetSelectedObject() { return selectedObject; }

	// Setters.
	void SetSceneObjects(std::vector<SceneObject*>* objects) { sceneObjects = objects; }
	void SetSelectedObject(SceneObject* object) { selectedObject = object; }

	void DeleteObject(SceneObject* target);
	virtual void Draw() override;

	ObjectsWindow(bool isActive = true, std::vector<SceneObject*>* objects = nullptr);
	~ObjectsWindow();
};