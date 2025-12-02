#pragma once
#include "SceneObject.h"

class Cube : public SceneObject
{
protected:
	bool move{ false };
	glm::vec3 movePosA{ 0.0f, 0.0f, 0.0f };
	glm::vec3 movePosB{ 0.0f, 0.0f, 0.0f };
	float moveSpeed{ 1.0f };
	bool moveReversed{ false };

public:
	void GUIExtras() override;
	void MoveBetweenPoints(float deltaTime);
	void Update(float deltaTime) override;
	void Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint gShaderProgram) override;

	Cube(std::string objectName = "Cube", glm::vec3 pos = {0.0f, 0.0f, 0.0f}, glm::vec3 rot = {0.0f, 0.0f, 0.0f}, glm::vec3 objectScale = {1.0f, 1.0f, 1.0f});
};