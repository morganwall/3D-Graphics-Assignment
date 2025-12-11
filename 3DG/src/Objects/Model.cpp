#include "Model.h"
#include <iostream>
#include <FreeImage.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

void Model::Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint gShaderProgram)
{

}

Model::Model(const std::string path) : SceneObject("Model")
{
	objectType = SceneObjectType::MODEL;

	// Create Assimp Importer.
	Assimp::Importer importer;

	// Read Model File.
	const aiScene* scene{ importer.ReadFile(path,
		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals | 
		aiProcess_JoinIdenticalVertices | aiProcess_ImproveCacheLocality)};

	// Check that Scene is Valid.
	if (!scene)
	{
		std::cout << "[!] Error loading model: " << importer.GetErrorString() << "\n";
		return;
	}

	// Get Directory from Path and Save it.
	//dir = path.substr(0, path.find_last_of('/'));

	//// Loop through Meshes.
	//for (int i = 0; i < scene->mRootNode->mNumMeshes; i++)
	//{
	//	// Get Mesh.
	//	aiMesh* mesh{ scene->mMeshes[scene->mRootNode->mMeshes[i]] };


	//}

	std::cout << "[+] Successfully loaded model: " << path << "\n";
	std::cout << "Meshes: " << scene->mNumMeshes << "\n";
	std::cout << "Materials: " << scene->mNumMaterials << "\n";
	std::cout << "Textures: " << scene->mNumTextures << "\n";
	std::cout << "Animations: " << scene->mNumAnimations << "\n";
}

Model::~Model()
{

}