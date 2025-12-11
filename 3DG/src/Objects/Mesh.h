#pragma once
#include "../EngineStructs.h"
#include <vector>

class Mesh
{
	std::vector<Engine::Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Engine::Texture> textures;
	GLuint VAO{ 0 }, VBO{ 0 }, EBO{ 0 };

public:
	void Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint gShaderProgram);

	Mesh(const std::vector<Engine::Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Engine::Texture> textures);
	~Mesh();
};