#include "Model.h"
#include <iostream>
#include <FreeImage.h>

GLuint Model::LoadTextureFromFile(const std::string& path)
{
	// Get File Format.
	FREE_IMAGE_FORMAT fileFormat{ FreeImage_GetFileType(path.c_str()) };
	if (fileFormat == FIF_UNKNOWN)
	{
		std::cout << "[!] Unknown File Type!\n";
		return 0;
	}

	// Load File.
	FIBITMAP* file{ FreeImage_Load(fileFormat, path.c_str()) };
	file = FreeImage_ConvertTo32Bits(file);
	if (!file)
	{
		std::cout << "[!] Failed to Load File!\n";
		return 0;
	}

	glm::ivec2 textureSize{ FreeImage_GetWidth(file), FreeImage_GetHeight(file) };
	BYTE* pixels{ FreeImage_GetBits(file) };

	// Generate Texture.
	GLuint textureID{ 0 };
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Set Sample Settings.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureSize.x, textureSize.y, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels); // Send pixel data to GPU.

	// Generate Mipmaps.
	glGenerateMipmap(GL_TEXTURE_2D);

	// Cleanup.
	FreeImage_Unload(file);
	glBindTexture(GL_TEXTURE_2D, 0);

	std::cout << "[+] Texture Loaded.\n";

	return textureID;
}

std::vector<Engine::Texture> Model::LoadTextures(aiMaterial* material)
{
	// Create Vector to Hold Textures.
	std::vector<Engine::Texture> textures;

	// Struct to Convert from Assimp Texture Types, to Our Texture Types.
	struct ConvertTypes
	{
		aiTextureType aiType;
		Engine::TextureType ourType;
	};

	ConvertTypes types[]
	{
		{aiTextureType_BASE_COLOR, Engine::TextureType::ALBEDO},
		{aiTextureType_DIFFUSE, Engine::TextureType::ALBEDO},
		{aiTextureType_NORMALS, Engine::TextureType::NORMAL},
		{aiTextureType_NORMAL_CAMERA, Engine::TextureType::NORMAL},
		{aiTextureType_DIFFUSE_ROUGHNESS, Engine::TextureType::ROUGHNESS},
		{aiTextureType_SHININESS, Engine::TextureType::ROUGHNESS},
		{aiTextureType_METALNESS, Engine::TextureType::METALLIC},
		{aiTextureType_SPECULAR, Engine::TextureType::METALLIC},
		{aiTextureType_AMBIENT_OCCLUSION, Engine::TextureType::AO},
		{aiTextureType_AMBIENT, Engine::TextureType::AO},
		{aiTextureType_LIGHTMAP, Engine::TextureType::AO},
		{aiTextureType_EMISSIVE, Engine::TextureType::EMISSIVE},
		{aiTextureType_HEIGHT, Engine::TextureType::HEIGHT},
		{aiTextureType_OPACITY, Engine::TextureType::OPACITY}
	};

	// Load Textures.
	for (ConvertTypes& curType : types)
	{
		// Get Number of Textures of this Type.
		unsigned int numTextures{ material->GetTextureCount(curType.aiType) };

		// Load Textures.
		for (int i = 0; i < numTextures; i++)
		{
			// Buffer to Hold Texture Path.
			aiString texturePath;

			// Get Texture.
			if (material->GetTexture(curType.aiType, i, &texturePath) == AI_SUCCESS)
			{
				// Create Full Path.
				std::string path{ dir + '/' + texturePath.C_Str() };

				Engine::Texture curTexture;
				curTexture.id = LoadTextureFromFile(path);
				curTexture.path = path;
				curTexture.type = curType.ourType;

				// Push to Textures Vector.
				textures.push_back(curTexture);
			}
		}
	}

	return textures;
}

void Model::GetNodes(const aiScene* scene, aiNode* node)
{
	// Loop through All Meshes in the Current Node.
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		// Get Current Mesh.
		aiMesh* curMesh{ scene->mMeshes[node->mMeshes[i]] };

		// Create Our Mesh Object from Model Mesh Data, and Push it to Meshes Vector.
		meshes.push_back(CreateMesh(scene, curMesh));
	}

	// Get Child Nodes.
	for (int i = 0; i < node->mNumChildren; i++)
		GetNodes(scene, node->mChildren[i]);
}

Mesh Model::CreateMesh(const aiScene* scene, aiMesh* mesh)
{
	// Store Mesh Data.
	std::vector<Engine::Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Engine::Texture> textures;

	// Loop through Mesh Vertices.
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		Engine::Vertex curVertex;

		// Get and Set Vertex Data.
		curVertex.position = asVec3(mesh->mVertices[i]);

		// Check that the Mesh has Normals Data.
		if (mesh->HasNormals())
			curVertex.normals = asVec3(mesh->mNormals[i]);
		else
			curVertex.normals = { 0.0f, 1.0f, 0.0f };

		// Check that the Mesh has Texture Data.
		if (mesh->mTextureCoords[0])
		{
			curVertex.UVs = { mesh->mTextureCoords[0][i].x, 1.0f - mesh->mTextureCoords[0][i].y };

			// Check that the Mesh has Tangent and Bitangent Data.
			if (mesh->HasTangentsAndBitangents())
			{
				curVertex.tangent = asVec3(mesh->mTangents[i]);
				curVertex.bitangent = asVec3(mesh->mBitangents[i]);
			}
			else
			{
				curVertex.tangent = { 1.0f, 0.0f, 0.0f };
				curVertex.bitangent = { 0.0f, 1.0f, 0.0f };
			}
		}
		else
		{
			curVertex.UVs = { 0.0f, 0.0f };
			curVertex.tangent = { 1.0f, 0.0f, 0.0f };
			curVertex.bitangent = { 0.0f, 1.0f, 0.0f };
		}

		vertices.push_back(curVertex);
	}

	// Loop through Mesh Faces to Get Indices.
	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace curFace{ mesh->mFaces[i] };

		// Loop through Indices.
		for (int j = 0; j < curFace.mNumIndices; j++)
			indices.push_back(curFace.mIndices[j]);
	}

	// Get Model Material.
	if (mesh->mMaterialIndex >= 0) // Check MaterialIndex, as this will give us the Number of Materials.
	{
		// Get the Material for the Current Mesh.
		aiMaterial* curMaterial{ scene->mMaterials[mesh->mMaterialIndex] };

		textures = LoadTextures(curMaterial);
	}

	return Mesh(vertices, indices, textures);
}

void Model::Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint gShaderProgram)
{
	SceneObject::Draw(viewMatrix, projectionMatrix, gShaderProgram);

	// Loop through Meshes.
	for (Mesh& curMesh : meshes)
		curMesh.Draw(viewMatrix, projectionMatrix, gShaderProgram);
}

Model::Model(const std::string path, glm::vec3 pos, glm::vec3 rot, glm::vec3 objectScale) : SceneObject(path.c_str(), pos, rot, objectScale)
{
	objectType = SceneObjectType::MODEL;

	// Get Directory from Path and Save it.
	dir = path.substr(0, path.find_last_of('/'));

	// Create Assimp Importer.
	Assimp::Importer importer;

	// Read Model File.
	const aiScene* scene{ importer.ReadFile(path,
		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals |
		aiProcess_JoinIdenticalVertices | aiProcess_ImproveCacheLocality) };

	// Check that Scene is Valid.
	if (!scene)
	{
		std::cout << "[!] Error loading model: " << importer.GetErrorString() << "\n";
		return;
	}

	// Get All Model Nodes.
	GetNodes(scene, scene->mRootNode);

	std::cout << "[+] Successfully loaded model: " << path << "\n";
}

Model::~Model()
{

}