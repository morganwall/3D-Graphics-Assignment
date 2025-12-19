#pragma once
#include <map>
#include <unordered_map>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "SceneObject.h"
#include "Mesh.h"

class Model : public SceneObject
{
	std::vector<Engine::Texture> textures;
	std::unordered_map<std::string, Engine::Texture> loadedTextures; // Texture Cache.
	std::vector<Mesh> meshes;
	std::string dir;

	GLuint LoadTextureFromFile(const std::string& path); // Load a texture from a file with FreeImage.
	std::vector<Engine::Texture> LoadTextures(aiMaterial* material); // Load all of the textures from an aiMaterial.
	void GetNodes(const aiScene* scene, aiNode* node); // Get all nodes from model. Recursive.
	Mesh CreateMesh(const aiScene* scene, aiMesh* mesh); // Create a mesh.

	// Vector Conversion Functions.
	glm::vec3 asVec3(aiVector3D& aiVec) { return glm::vec3(aiVec.x, aiVec.y, aiVec.z); }
	glm::vec2 asVec2(aiVector2D& aiVec) { return glm::vec2(aiVec.x, aiVec.y); }

public:
	void GUIExtras() override;
	void Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint gShaderProgram);

	Model(const std::string path, glm::vec3 pos = { 0.0f, 0.0f, 0.0f }, glm::vec3 rot = { 0.0f, 0.0f, 0.0f }, glm::vec3 objectScale = { 1.0f, 1.0f, 1.0f });
	~Model();
};