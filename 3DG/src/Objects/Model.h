#pragma once
#include <map>
#include "SceneObject.h"
#include "Mesh.h"

class Model : public SceneObject
{
	std::vector<Mesh> meshes;
	std::map<std::string, Engine::Texture> textures;
	std::string dir;

	//bool LoadModel(const std::string path);
	//void HandleNode(aiNode* node, const aiScene* scene);

public:
	void Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint gShaderProgram);

	Model(const std::string path);
	~Model();
};