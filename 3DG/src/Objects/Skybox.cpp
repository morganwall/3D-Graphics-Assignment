#include "Skybox.h"

bool Skybox::LoadCubemap()
{

}

void Skybox::GUIExtras()
{

}

void Skybox::Update(float deltaTime)
{
	Cube::Update(deltaTime);
}

void Skybox::Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint gShaderProgram)
{

}

Skybox::Skybox(std::string objectName, glm::vec3 pos, glm::vec3 rot, std::string cubemap) : Cube(objectName, pos, rot), cubemapPath(cubemap)
{
	// Set Object Type.
	objectType = SceneObjectType::SKYBOX;
}