#include "Mesh.h"
#include <glew.h>

void Mesh::Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint gShaderProgram)
{
	// Keep Track of What Textures we Have.
	bool hasAlbedo{ false };
	bool hasNormal{ false };
	bool hasRoughness{ false };
	bool hasMetallic{ false };
	bool hasAO{ false };
	bool hasEmissive{ false };
	bool hasHeight{ false };
	bool hasOpacity{ false };

	// Bind Textures.
	for (int i = 0; i < textures.size(); i++)
	{
		// Activate Texture.
		glActiveTexture(GL_TEXTURE0 + i);

		std::string textureName{ "" };

		switch (textures[i].type)
		{
		case Engine::TextureType::ALBEDO:
			hasAlbedo = true;
			textureName = "material.albedo";
			break;
		case Engine::TextureType::NORMAL:
			hasNormal = true;
			textureName = "material.normal";
			break;
		case Engine::TextureType::ROUGHNESS:
			hasRoughness = true;
			textureName = "material.roughness";
			break;
		case Engine::TextureType::METALLIC:
			hasMetallic = true;
			textureName = "material.metallic";
			break;
		case Engine::TextureType::AO:
			hasAO = true;
			textureName = "material.ao";
			break;
		case Engine::TextureType::EMISSIVE:
			hasEmissive = true;
			textureName = "material.emissive";
			break;
		case Engine::TextureType::HEIGHT:
			hasHeight = true;
			textureName = "material.height";
			break;
		case Engine::TextureType::OPACITY:
			hasOpacity = true;
			textureName = "material.opacity";
			break;
		default:
			break;
		}

		// Send Texture to Shader.
		glUniform1i(glGetUniformLocation(gShaderProgram, textureName.c_str()), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	// Bind Vertex Array.
	glBindVertexArray(VAO);

	// Draw Elements.
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	// Unbind Vertex Array.
	glBindVertexArray(VAO);

	// Cleanup Textures.
	for (int i = 0; i < textures.size(); i++)
	{
		// Get the Active Texture.
		glActiveTexture(GL_TEXTURE0 + i);

		// Unbind Texture.
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

Mesh::Mesh(const std::vector<Engine::Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Engine::Texture> textures) : vertices(vertices), indices(indices), textures(textures)
{
	// Populate Objects.
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind VAO.
	glBindVertexArray(VAO);

	// Set Vertex Buffer.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices.front()), &vertices.front(), GL_STATIC_DRAW);

	// Set Element Buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices.front()), &indices.front(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Set Vertex Data.
	// Position.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Engine::Vertex), (void*)offsetof(Engine::Vertex, position)); // Offsetof gets the byte offset between members of a data structure. https://cplusplus.com/reference/cstddef/offsetof/

	// UVs.
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Engine::Vertex), (void*)offsetof(Engine::Vertex, UVs));

	// Normals.
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Engine::Vertex), (void*)offsetof(Engine::Vertex, normals));

	// Tangent.
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Engine::Vertex), (void*)offsetof(Engine::Vertex, tangent));

	// Bitangent.
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Engine::Vertex), (void*)offsetof(Engine::Vertex, bitangent));

	// Unbind Vertex Array.
	glBindVertexArray(0);
}

Mesh::~Mesh()
{

}