#pragma once
#include "SceneObject.h"

#include "FreeImage.h"

enum class SkyboxMode
{
	CUBEMAP,
	COLOUR
};

class Skybox : public SceneObject
{
	GLuint cubemapID{ 0 };
	std::vector<std::string> faces
	{
		"right.jpg", "left.jpg", // +X, -X,
		"top.jpg", "bottom.jpg", // +Y, -Y,
		"front.jpg", "back.jpg"  // +Z, -Z
	};
	std::string cubemapDir{ "Default" };
	SkyboxMode mode{ SkyboxMode::CUBEMAP };
	glm::vec3 colour{ 0.5f, 0.5f, 0.5f };
	char cubemapDirBuffer[128];

public:
	void GUIExtras() override;
	void Update(float deltaTime) override;
	void Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint gShaderProgram) override;

	bool LoadCubemap();

	Skybox(std::string objectName = "Skybox", std::string cubemapName = "Default", SkyboxMode skyboxMode = SkyboxMode::CUBEMAP, glm::vec3 skyboxColour = { 0.5f, 0.5f, 0.5f });
	~Skybox();
};