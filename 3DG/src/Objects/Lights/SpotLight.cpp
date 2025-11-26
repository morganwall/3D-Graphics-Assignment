#include "SpotLight.h"

void SpotLight::GUIExtras()
{
	Light::GUIExtras();

	ImGui::SeparatorText("Spot Light Settings");
	if (ImGui::DragFloat("Attenuation", &attenuation, 0.1f))
		if (attenuation < 0.0f)
			attenuation = 0.0f;
	ImGui::DragFloat("Inner Cone", &innerCone, 0.5f, 0.0f, 180.0f);
	ImGui::DragFloat("Outer Cone", &outerCone, 0.5f, 0.0f, 180.0f);
	if (ImGui::DragFloat("Max Range", &maxRange))
		if (maxRange < 0.0f)
			maxRange = 0.0f;
}

void SpotLight::Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint gShaderProgram)
{

}

void SpotLight::ShaderSetup(GLuint gShaderProgram)
{
	// Convert Our Rotation to Radians to be Used as the Light's Direction.
	float pitch{ glm::radians(rotation.x) };
	float yaw{ glm::radians(rotation.y) };

	glm::vec3 direction{ cos(yaw) * cos(pitch), sin(pitch), sin(yaw) * cos(pitch) };

	glUniform1i(glGetUniformLocation(gShaderProgram, "light.enabled"), active);
	glUniform3f(glGetUniformLocation(gShaderProgram, "light.position"), position.x, position.y, position.z);
	glUniform1f(glGetUniformLocation(gShaderProgram, "light.intensity"), intensity);
	glUniform1i(glGetUniformLocation(gShaderProgram, "light.type"), (int)type);
	glUniform3f(glGetUniformLocation(gShaderProgram, "light.colour"), colour.r, colour.g, colour.b);
	glUniform1f(glGetUniformLocation(gShaderProgram, "light.innerCone"), innerCone);
	glUniform1f(glGetUniformLocation(gShaderProgram, "light.outerCone"), outerCone);
	glUniform3f(glGetUniformLocation(gShaderProgram, "light.direction"), direction.x, direction.y, direction.z);
	glUniform1f(glGetUniformLocation(gShaderProgram, "light.attenuation"), attenuation);
	glUniform1f(glGetUniformLocation(gShaderProgram, "light.maxRange"), maxRange);
}

SpotLight::SpotLight(std::string objectName, glm::vec3 pos, glm::vec3 rot) : Light(objectName, LightType::Spot, pos, rot)
{
	// Set Object Type.
	objectType = SceneObjectType::LIGHT;
}