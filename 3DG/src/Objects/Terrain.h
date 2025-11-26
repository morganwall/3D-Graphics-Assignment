#pragma once
#include <vector>
#include "SceneObject.h"
#include "FreeImage.h"

enum class TriangleMode
{
	Alternating,
	Uniform
};

enum class HeightMode
{
	Map,
	Generated
};

class Terrain : public SceneObject
{
	void UpdateEBO();

protected:
	glm::ivec2 size{ 32, 32 };
	glm::vec2 triangleSize{ 1.0f, 1.0f };
	TriangleMode triangleMode{ TriangleMode::Alternating };
	std::string heightMapPath{ "Data/Heightmaps/curvy.gif" };
	FIBITMAP* heightMap{ nullptr };
	glm::ivec2 heightMapSize{ 0, 0 };
	float heightScale{ 6.0f };
	HeightMode heightMode{ HeightMode::Map };
	GLuint uvbo{ 0 };
	FIBITMAP* texture{ nullptr };
	glm::ivec2 textureSize{ 0,0 };
	GLuint textureID{ 0 };
	std::string texturePath{ "Data/Textures/dirt_earth-n-moss_df_.dds" };

public:
	bool LoadHeightmap(const std::string& path);
	bool LoadTexture(const std::string& path);
	void GUIExtras() override;
	void Generate();
	void Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint gShaderProgram) override;

	Terrain(std::string objectName = "Terrain", TriangleMode triMode = TriangleMode::Alternating, HeightMode heightGenMode = HeightMode::Map, glm::ivec2 terrainSize = {32, 32}, glm::vec2 triSize = {1.0f, 1.0f}, 
		std::string heightMap = "Data/Heightmaps/curvy.gif", std::string texture = "Data/Textures/dirt_earth-n-moss_df_.dds");
	~Terrain();
};