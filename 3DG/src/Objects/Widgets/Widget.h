#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glew.h>

enum class WidgetType
{
	WIDGET,
	TRANSFORM,
	LIGHT
};

class Widget
{
protected:
	bool active{ true };
	glm::vec3 position{ 0.0f, 0.0f, 0.0f };
	float size{ 1.0f };
	WidgetType type{ WidgetType::WIDGET };

public:
	virtual void GUI();
	virtual void Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint gShaderProgram);

	// Getters.
	bool GetActive() { return active; }
	glm::vec3 GetPosition() { return position; }
	float GetSize() { return size; }

	// Setters.
	void SetActive(bool isActive) { active = isActive; }
	void SetPosition(glm::vec3 newPosition) { position = newPosition; }
	void SetSize(float newSize) { size = newSize; }

	Widget(bool visible = false, float widgetSize = 1.0f, WidgetType widgetType = WidgetType::WIDGET);
	virtual ~Widget();
};