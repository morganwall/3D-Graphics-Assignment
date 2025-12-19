#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glew.h>
#include <imgui.h>

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
		UNKNOWN,
		ALBEDO, // Base colour.
		NORMAL, // Surface detail.
		ROUGHNESS, // Shiny/Rough.
		METALLIC, // Non-Metallic/Metallic.
		AO, // Ambient occlusion. Shadows in crevices and corners.
		EMISSIVE, // Illumination.
		HEIGHT, // Displacement.
		OPACITY // Transparency.
	};

	std::string TextureTypeToString(TextureType type);

	struct Texture
	{
		GLuint id = 0; // The texture's ID.
		std::string path = ""; // The texture's path.
		TextureType type = TextureType::UNKNOWN; // The type of texture.

		Texture(GLuint id = 0, std::string path = "", TextureType type = TextureType::UNKNOWN) : id(id), path(path), type(type) {}
	};

	enum class LogType
	{
		INFO,
		ERROR,
		SUCCESS,
		CUSTOM
	};

	struct LoggerMsg
	{
		std::string name = "Name";
		std::string description = "Description";
		LogType type = LogType::INFO;
		ImColor colour = { 1.0f, 1.0f, 1.0f, 1.0f };
		bool toGUI = true;
		bool toConsole = true;

		LoggerMsg(std::string name = "Name", std::string description = "Description", LogType type = LogType::INFO, ImColor colour = {1.0f, 1.0f, 1.0f, 1.0f}, bool toGUI = true, bool toConsole = true) : name(name), description(description), type(type), colour(colour), toGUI(toGUI), toConsole(toConsole) {}
	};
}