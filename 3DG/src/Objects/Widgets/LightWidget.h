#pragma once
#include "Widget.h"
#include <iostream>
#include <vector>

class LightWidget : public Widget
{
	glm::vec3 colour{ 1.0f, 1.0f, 1.0f };
	GLuint vao{ 0 }, // Vertex array object.
	vbo{ 0 }, // Vertex buffer object.
	ebo{ 0 }; // Element buffer object.
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;


public:
	void GUI() override;
	void Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint gShaderProgram) override;

	// Getters.
	glm::vec3 GetColour() { return colour; }

	// Setters.
	void SetColour(glm::vec3 widgetColour) { colour = widgetColour; }

	LightWidget(bool visible = false, float widgetSize = 1.0f, WidgetType widgetType = WidgetType::LIGHT);
	~LightWidget();
};