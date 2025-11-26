#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
	void GUIExtras() override;
	void Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint gShaderProgram) override;
	void ShaderSetup(GLuint gShaderProgram) override;

	DirectionalLight(std::string objectName = "Directional Light", glm::vec3 pos = {0.0f, 0.0f, 0.0f}, glm::vec3 rot = {0.0f, 0.0f, 0.0f});
};