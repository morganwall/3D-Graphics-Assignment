#include "Terrain.h"

void Terrain::UpdateEBO()
{
	// Update EBO.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices.front()), indices.data(), GL_STATIC_DRAW);
}

bool Terrain::LoadHeightmap(const std::string& path)
{
	// Initialise FreeImage.
	FreeImage_Initialise();

	// Get File Type from Path String.
	FREE_IMAGE_FORMAT fileFormat{ FreeImage_GetFileType(path.c_str()) };
	if (fileFormat == FIF_UNKNOWN)
	{
		std::cout << "[!] Unknown File Type!\n";
		std::cout << "[!] Failed to Load Height Map!\n";
		return false;
	}

	// Load File.
	heightMap = FreeImage_Load(fileFormat, path.c_str());
	heightMap = FreeImage_ConvertTo24Bits(heightMap);
	if (!heightMap)
	{
		std::cout << "[!] Failed to Load Height Map!\n";
		heightMap = nullptr; // Ensuring that it's still nullptr.
		return false;
	}
	heightMapSize = { FreeImage_GetWidth(heightMap), FreeImage_GetHeight(heightMap) };

	std::cout << "[+] Height Map Loaded.\n";
	return true;
}

bool Terrain::LoadTexture(const std::string& path)
{
	// Get File Type from Path String.
	FREE_IMAGE_FORMAT fileFormat{ FreeImage_GetFileType(path.c_str()) };
	if (fileFormat == FIF_UNKNOWN)
	{
		std::cout << "[!] Unknown File Type!\n";
		std::cout << "[!] Failed to Load Texture!\n";
		return false;
	}

	// Load File.
	texture = FreeImage_Load(fileFormat, path.c_str());
	texture = FreeImage_ConvertTo32Bits(texture);
	if (!texture)
	{
		std::cout << "[!] Failed to Load Texture!\n";
		texture = nullptr; // Ensuring that it's still nullptr.
		return false;
	}
	textureSize = { FreeImage_GetWidth(texture), FreeImage_GetHeight(texture) };
	BYTE* pixels{ FreeImage_GetBits(texture) };

	// Generate Texture.
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureSize.x, textureSize.y, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels); // Send pixel data to GPU.

	// Set Sample Settings.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	FreeImage_Unload(texture);

	std::cout << "[+] Texture Loaded.\n";
	return true;
}

void Terrain::GUIExtras()
{
	ImGui::SeparatorText("Triangle Settings");

	if (ImGui::DragFloat2("Triangle Size", &triangleSize.x))
	{
		// Clamp.
		if (triangleSize.x < 0.1f)
			triangleSize.x = 0.1f;
		if (triangleSize.y < 0.1f)
			triangleSize.y = 0.1f;

		Generate();

		// Set Vertex Data.
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices.front()), vertices.data(), GL_STATIC_DRAW);
		UpdateEBO();
	}

	const char* triModeItems[] = { "Alternating", "Uniform" };
	int triModeItemCur = static_cast<int>(triangleMode);
	if (ImGui::Combo("Triangle Mode", &triModeItemCur, triModeItems, IM_ARRAYSIZE(triModeItems)))
	{
		triangleMode = static_cast<TriangleMode>(triModeItemCur);
		Generate(); // Regenerate mesh with new triangle mode.
		UpdateEBO();
	}

	ImGui::SeparatorText("Height Settings");

	const char* hModeItems[] = { "Map", "Generated" };
	int hModeItemCur = static_cast<int>(heightMode);
	if (ImGui::Combo("Mode", &hModeItemCur, hModeItems, IM_ARRAYSIZE(hModeItems)))
	{
		heightMode = static_cast<HeightMode>(hModeItemCur);
	}

	if (ImGui::DragFloat("Scale", &heightScale))
	{
		Generate();

		// Set Vertex Data.
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices.front()), vertices.data(), GL_STATIC_DRAW);

		// Set Normal Data.
		glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normals.front()), normals.data(), GL_STATIC_DRAW);

		UpdateEBO();
	}
}

void Terrain::Generate()
{
	// Clear Vectors.
	vertices.clear();
	indices.clear();
	colours.clear();
	uvs.clear();
	normals.clear();

	normals.resize((size.x + 1) * (size.y + 1), { 0.0f, 0.0f, 0.0f });

	for (int z = 0; z <= size.y; z++)
	{
		for (int x = 0; x <= size.x; x++)
		{
			// Get Vertex Pos.
			glm::vec3 vertexPos{ x * triangleSize.x, 0.0f, z * triangleSize.y };

			// Check if we have a Height Map Loaded.
			if (heightMap)
			{
				glm::vec2 samplePoint{ (x * (heightMapSize.x - 1)) / (size.x - 1), (z * (heightMapSize.y - 1)) / (size.y - 1) };

				// Sample Height Map for Vertex Height.
				RGBQUAD samplePixel;
				FreeImage_GetPixelColor(heightMap, samplePoint.x, samplePoint.y, &samplePixel); // Get the colour of the pixel at the position of our vertex.

				// Set Vertex Height.
				vertexPos.y = (samplePixel.rgbRed / 255.0f) * heightScale; // Get the pixel colour as a float.
			}

			// Create Vertex.
			vertices.push_back(vertexPos);

			// Create UVs.
			uvs.push_back({ (float)x / (size.x - 1), (float)z / (size.y - 1) });
		}
	}

	for (int z = 0; z < size.y; z++)
	{
		for (int x = 0; x < size.x; x++)
		{
			// Create Indices.
			int topLeft{ z * (size.x + 1) + x };
			int topRight{ topLeft + 1 };
			int bottomLeft{ (z + 1) * (size.x + 1) + x };
			int bottomRight{ bottomLeft + 1 };

			// Check if we should Flip the Current Triangle. Used for Alternate Triangle Mode.
			if (triangleMode == TriangleMode::Alternating && ((x + z) % 2 == 1))
			{
				// Triangle One.
				indices.push_back(topLeft);
				indices.push_back(bottomLeft);
				indices.push_back(topRight);

				// Triangle Two.
				indices.push_back(topRight);
				indices.push_back(bottomLeft);
				indices.push_back(bottomRight);
			}
			else
			{
				indices.push_back(topLeft);
				indices.push_back(bottomRight);
				indices.push_back(topRight);

				indices.push_back(topLeft);
				indices.push_back(bottomLeft);
				indices.push_back(bottomRight);
			}

			// Create Colour. TEMP.
			colours.push_back({ (float)(x / size.x), (float)(z / size.y), 0.2f });
			colours.push_back({ (float)(x / size.x), (float)(z / size.y), 0.8f });
		}
	}

	glm::vec3 triEdge1;
	glm::vec3 triEdge2;
	glm::vec3 edgeCross;

	// Calculate Normals.
	for (size_t i = 0; i < indices.size(); i += 3) // Stride of 3.
	{
		// Calculate Triangle Edges.
		triEdge1 = vertices[indices[i + 1]] - vertices[indices[i]];
		triEdge2 = vertices[indices[i + 2]] - vertices[indices[i]];

		edgeCross = glm::cross(triEdge1, triEdge2);

		normals[indices[i]] += edgeCross;
		normals[indices[i + 1]] += edgeCross;
		normals[indices[i + 2]] += edgeCross;
	}
	
	// Normalise.
	for (size_t i = 0; i < normals.size(); i++)
		normals[i] = glm::normalize(normals[i]);
}

void Terrain::Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint gShaderProgram)
{
	SceneObject::Draw(viewMatrix, projectionMatrix, gShaderProgram);

	// Use Texture.
	glUniform1i(glGetUniformLocation(gShaderProgram, "hasTexture"), 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(glGetUniformLocation(gShaderProgram, "terrainTexture"), 0);

	// Draw Faces with Colours.
	GLint colourLocation{ glGetUniformLocation(gShaderProgram, "baseColour") };
	for (size_t i = 0; i < colours.size(); i++)
	{
		glUniform3fv(colourLocation, 1, glm::value_ptr(colours[i]));
		glDrawElements(GL_TRIANGLES, numIndicesPerFace, GL_UNSIGNED_INT, (void*)(i * numIndicesPerFace * sizeof(GLuint)));
	}

	glBindVertexArray(0); // Unbind Vertex Array.
}

Terrain::Terrain(std::string objectName, TriangleMode triMode, HeightMode heightGenMode, glm::ivec2 terrainSize, glm::vec2 triSize, std::string heightMap, std::string texture) : SceneObject(objectName), triangleMode(triMode),
size(terrainSize), triangleSize(triSize), heightMode(heightGenMode), heightMapPath(heightMap), texturePath(texture)
{
	// Set Object Type.
	objectType = SceneObjectType::TERRAIN;

	LoadHeightmap(heightMapPath);
	Generate();
	LoadTexture(texturePath);

	// Populate Objects.
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &uvbo);
	glGenBuffers(1, &ebo);
	glGenBuffers(1, &normalBuffer);
	glBindVertexArray(vao);

	// Set Vertex Data.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices.front()), vertices.data(), GL_STATIC_DRAW);

	// Set UV Data.
	glBindBuffer(GL_ARRAY_BUFFER, uvbo);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(uvs.front()), uvs.data(), GL_STATIC_DRAW);

	// Set Index Data.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices.front()), indices.data(), GL_STATIC_DRAW);

	// Set Normal Data.
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normals.front()), normals.data(), GL_STATIC_DRAW);

	// Set Vertex Attributes.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // Position.
	glEnableVertexAttribArray(0);

	// Set UV Attributes.
	glBindBuffer(GL_ARRAY_BUFFER, uvbo);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); // UV.
	glEnableVertexAttribArray(1);

	// Set Normal Attributes.
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // Normals.
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	std::cout << "[+] Terrain Created.\n";
}

Terrain::~Terrain()
{
	// Cleanup Texture.
	glDeleteTextures(1, &textureID);
}