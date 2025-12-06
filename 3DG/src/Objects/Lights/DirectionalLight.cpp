#include "DirectionalLight.h"
#include "../Widgets/LightWidget.h"

void DirectionalLight::GUIExtras()
{
	Light::GUIExtras();
}

void DirectionalLight::Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint gShaderProgram)
{

}

void DirectionalLight::ShaderSetup(GLuint gShaderProgram)
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
	glUniform3f(glGetUniformLocation(gShaderProgram, "light.direction"), direction.x, direction.y, direction.z);
}

DirectionalLight::DirectionalLight(std::string objectName, glm::vec3 pos, glm::vec3 rot) : Light(objectName, LightType::DIRECTIONAL, pos, rot)
{
	// Set Object Type.
	objectType = SceneObjectType::LIGHT;

	// Create Light Widget.
	widgets.push_back(std::make_unique<LightWidget>(true, 0.25f, WidgetType::LIGHT, LightType::DIRECTIONAL));
}