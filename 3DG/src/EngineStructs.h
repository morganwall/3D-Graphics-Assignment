#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glew.h>

namespace Engine
{
	struct Vertex
	{
		glm::vec3 position; // Model space position.
		glm::vec2 UVs; // The texture coordinates.
		glm::vec3 normals; // For lighting.
		glm::vec3 tangent; // For normal mapping.
		glm::vec3 bitangent; // For normal mapping.
	};

	enum class TextureType
	{
		ALBEDO, // Base colour.
		NORMAL, // Surface detail.
		ROUGHNESS, // Shiny/Rough.
		METALLIC, // Non-Metallic/Metallic.
		AO, // Ambient occlusion. Shadows in crevices and corners.
		EMISSIVE, // Illumination.
		HEIGHT, // Displacement.
		OPACITY // Transparency.
	};

	struct Texture
	{
		GLuint id; // The texture's ID.
		std::string path = ""; // The texture's path.
		TextureType type = TextureType::ALBEDO; // The type of texture.
	};
}