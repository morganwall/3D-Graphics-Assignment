#pragma once
#include "Widget.h"
#include <iostream>
#include <vector>

class TransformWidget : public Widget
{
	glm::vec3 colours[3] =
	{
		{1.0f, 0.0f, 0.0f}, // X.
		{0.0f, 1.0f, 0.0f}, // Y.
		{0.0f, 0.0f, 1.0f}  // Z.
	};
	GLuint vao{ 0 }, // Vertex array object.
	vbo{ 0 }, // Vertex buffer object.
	ebo{ 0 }; // Element buffer object.
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;

public:
	void GUI() override;
	void Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint gShaderProgram) override;

	TransformWidget();
	~TransformWidget();
};