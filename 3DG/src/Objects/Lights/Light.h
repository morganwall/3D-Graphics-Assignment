#pragma once
#include "../SceneObject.h"
#include "../Widgets/LightWidget.h"

enum class LightType
{
	DIRECTIONAL,
	POINT,
	SPOT
};

class Light : public SceneObject
{
protected:
	// Light.
	LightType type{ LightType::DIRECTIONAL };
	glm::vec3 colour{ 1.0f, 1.0f, 1.0f };
	float intensity{ 1.0f };

public:
	void GUIExtras() override;
	void Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint gShaderProgram) override;
	virtual void Update(float deltaTime) override;
	virtual void ShaderSetup(GLuint gShaderProgram);

	// Getters.
	LightType GetType() { return type; }
	glm::vec3 GetColour() { return colour; }
	float GetIntensity() { return intensity; }

	// Setters.
	void SetType(LightType lightType) { type = lightType; }
	void SetColour(glm::vec3 lightColour) { colour = lightColour; }
	void SetIntensity(float lightIntensity) { intensity = lightIntensity; }

	Light(std::string objectName, LightType type = LightType::POINT, glm::vec3 pos = { 0.0f, 0.0f, 0.0f }, glm::vec3 rot = { 0.0f, 0.0f, 0.0f });
};