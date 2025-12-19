#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glew.h>
#include <imgui.h>
#include "Widgets/Widget.h"
#include "Widgets/TransformWidget.h"

enum class SceneObjectType
{
	SCENE_OBJECT,
	CUBE,
	TERRAIN,
	CAMERA,
	LIGHT,
	SKYBOX,
	MODEL
};

class SceneObject
{
protected:
	bool active{ true }; // Is the object active in the scene.
	bool hasMesh{ true }; // Does the object have a mesh to render.
	std::string name{ "Object" }; // Object's name.
	glm::vec3 position{ 0.0f, 0.0f, 0.0f }; // Object's world position.
	glm::vec3 rotation{ 0.0f, 0.0f, 0.0f }; // Object's rotation.
	glm::vec3 scale{ 1.0f, 1.0f, 1.0f  };
	GLuint vao{ 0 }, // Vertex array object.
	vbo{ 0 }, // Vertex buffer object.
	ebo{ 0 }; // Element buffer object.
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	float shininess{ 32.0f }; // Specular shininess factor.
	GLuint normalBuffer{ 0 };
	GLsizei numIndicesPerFace{ 6 }; // Number of indices per face.
	bool wireframe{ false }; // Show the object's wireframe.
	std::vector<glm::vec3> colours;
	SceneObjectType objectType{ SceneObjectType::SCENE_OBJECT }; // Pretty much the object type ID. An easier way of telling what type each object is. Used mostly for the GUI.
	std::vector<std::unique_ptr<Widget>> widgets;

public:
	void DrawWidgets(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint gShaderProgram, bool isSelected);
	virtual void Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint gShaderProgram);
	virtual void Update(float deltaTime);
	void GUI();
	virtual void GUIExtras();
	void Cleanup();

	// Getters.
	bool GetActive() { return active; }
	bool GetHasMesh() { return hasMesh; }
	std::string GetName() { return name; }
	glm::vec3 GetPosition() { return position; }
	glm::vec3 GetRotation() { return rotation; }
	glm::vec3 GetScale() { return scale; }
	SceneObjectType GetObjectType() { return objectType; }

	// Setters.
	void SetActive(bool isActive) { active = isActive; }
	void SetName(std::string newName) { name = newName; }
	void SetPosition(glm::vec3 pos) { position = pos; }
	void SetRotation(glm::vec3 rot) { rotation = rot; }
	void SetScale(glm::vec3 newScale) { scale = newScale; }
	void SetObjectType(SceneObjectType type) { objectType = type; }

	SceneObject(std::string objectName, glm::vec3 pos = { 0.0f, 0.0f, 0.0f }, glm::vec3 rot = { 0.0f, 0.0f, 0.0f }, glm::vec3 objectScale = { 1.0f, 1.0f, 1.0f });
	virtual ~SceneObject();
};