#include "PointLight.h"

void PointLight::GUIExtras()
{
	Light::GUIExtras();

	ImGui::SeparatorText("Point Light Settings");
	if (ImGui::DragFloat("Attenuation", &attenuation, 0.1f))
		if (attenuation < 0.0f)
			attenuation = 0.0f;
	if (ImGui::DragFloat("Max Range", &maxRange))
		if (maxRange < 0.0f)
			maxRange = 0.0f;
}

void PointLight::Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint gShaderProgram)
{

}

void PointLight::ShaderSetup(GLuint gShaderProgram)
{
	glUniform1i(glGetUniformLocation(gShaderProgram, "light.enabled"), active);
	glUniform3f(glGetUniformLocation(gShaderProgram, "light.position"), position.x, position.y, position.z);
	glUniform1f(glGetUniformLocation(gShaderProgram, "light.intensity"), intensity);
	glUniform1i(glGetUniformLocation(gShaderProgram, "light.type"), (int)type);
	glUniform3f(glGetUniformLocation(gShaderProgram, "light.colour"), colour.r, colour.g, colour.b);
	glUniform1f(glGetUniformLocation(gShaderProgram, "light.attenuation"), attenuation);
	glUniform1f(glGetUniformLocation(gShaderProgram, "light.maxRange"), maxRange);
}

PointLight::PointLight(std::string objectName, glm::vec3 pos, glm::vec3 rot) : Light(objectName, LightType::POINT, pos, rot)
{
	// Set Object Type.
	objectType = SceneObjectType::LIGHT;
}