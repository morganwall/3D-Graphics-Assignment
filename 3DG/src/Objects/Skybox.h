#pragma once
#include "Cube.h"

enum class SkyboxMode
{
	COLOUR,
	CUBEMAP
};

class Skybox : public Cube
{
	bool useCubemap{ false };
	GLuint cubemapID{ 0 };
	std::string cubemapPath{ "Data/Cubemaps/radkow_lake_4k.exr" };

public:
	bool LoadCubemap();
	void GUIExtras() override;
	void Update(float deltaTime) override;
	void Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint gShaderProgram) override;

	Skybox(std::string objectName = "Skybox", glm::vec3 pos = { 0.0f, 0.0f, 0.0f }, glm::vec3 rot = { 0.0f, 0.0f, 0.0f }, std::string cubemap = "Data/Cubemaps/radkow_lake_4k.exr");
	~Skybox() {}
};