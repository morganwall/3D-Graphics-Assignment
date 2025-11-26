#pragma once
#include "Light.h"

class PointLight : public Light
{
	float attenuation{ 1.0f };
	float maxRange{ 100.0f };

public:
	void GUIExtras() override;
	void Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint gShaderProgram) override;
	void ShaderSetup(GLuint gShaderProgram) override;

	// Getters.
	float GetAttenuation() { return attenuation; }
	float GetMaxRange() { return maxRange; }

	// Setters.
	void SetAttenuation(float distance) { attenuation = distance; }
	void SetMaxRange(float range) { maxRange = range; }

	PointLight(std::string objectName = "Point Light", glm::vec3 pos = {0.0f, 0.0f, 0.0f}, glm::vec3 rot = {0.0f, 0.0f, 0.0f});
};