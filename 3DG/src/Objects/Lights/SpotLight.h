#pragma once
#include "Light.h"

class SpotLight : public Light
{
	float attenuation{ 1.0f };
	float innerCone{ 10.0f };
	float outerCone{ 15.0f };
	float maxRange{ 100.0f };

public:
	void GUIExtras() override;
	void Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint gShaderProgram) override;
	void ShaderSetup(GLuint gShaderProgram) override;

	// Getters.
	float GetAttenuation() { return attenuation; }
	float GetInnerCone() { return innerCone; }
	float GetOuterCone() { return outerCone; }
	float GetMaxRange() { return maxRange; }

	// Setters.
	void SetAttenuation(float distance) { attenuation = distance; }
	void SetInnerCone(float angle) { innerCone = angle; }
	void SetOuterCone(float angle) { outerCone = angle; }
	void SetMaxRange(float range) { maxRange = range; }

	SpotLight(std::string objectName = "Spot Light", glm::vec3 pos = {0.0f, 0.0f, 0.0f}, glm::vec3 rot = {0.0f, 0.0f, 0.0f});
};