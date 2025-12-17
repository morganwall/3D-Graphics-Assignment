#include "Skybox.h"
#include "../Logger.h"

extern Logger* logger;

void Skybox::GUIExtras()
{
	ImGui::SeparatorText("Skybox Settings");

	const char* modeItems[] = { "Cubemap", "Colour" };
	int modeItemCur = static_cast<int>(mode);
	if (ImGui::Combo("Skybox Mode", &modeItemCur, modeItems, IM_ARRAYSIZE(modeItems)))
	{
		mode = static_cast<SkyboxMode>(modeItemCur);
	}

	if (mode == SkyboxMode::CUBEMAP)
	{
		ImGui::SeparatorText("Cubemap Settings");

		ImGui::InputText("Cubemap Name", cubemapDirBuffer, sizeof(cubemapDirBuffer));
		if (ImGui::Button("Apply"))
		{
			cubemapDir = std::string(cubemapDirBuffer);
			LoadCubemap();
		}
		ImGui::SameLine();
		if (ImGui::Button("Reset"))
		{
			strncpy_s(cubemapDirBuffer, cubemapDir.c_str(), sizeof(cubemapDirBuffer) - 1);
			cubemapDirBuffer[sizeof(cubemapDirBuffer) - 1] = '\0';
		}
	}
	else
	{
		ImGui::SeparatorText("Colour Settings");

		ImGui::PushItemWidth(150.0f);
		ImGui::ColorPicker3("Skybox Colour", &colour.x, ImGuiColorEditFlags_DisplayRGB);
		ImGui::PopItemWidth();
	}
}

void Skybox::Update(float deltaTime)
{
	SceneObject::Update(deltaTime);
}

void Skybox::Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint gShaderProgram)
{
	if (!active)
		return;

	glDepthFunc(GL_LEQUAL);

	// Check if we should Draw as Wireframe.
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUseProgram(gShaderProgram);

	// Create Model Matrix.
	glm::mat4 modelMatrix{ glm::translate(glm::mat4(1.0f), position) };

	// Rotate Object.
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	// Scale Object.
	modelMatrix = glm::scale(modelMatrix, scale);

	// Remove the Translation from the View Matrix.
	glm::mat4 viewMatrixNoTranslate{ glm::mat4(glm::mat3(viewMatrix)) };

	// Upload Matrices to Shader.
	glUniformMatrix4fv(glGetUniformLocation(gShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(gShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrixNoTranslate));
	glUniformMatrix4fv(glGetUniformLocation(gShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	// Bind Vertex Array.
	glBindVertexArray(vao);

	// Pass Mode to Shader.
	glUniform1i(glGetUniformLocation(gShaderProgram, "skyboxMode"), static_cast<int>(mode));

	// Check Mode.
	if (mode == SkyboxMode::CUBEMAP)
	{
		// Pass Cubemap Texture to Shader.
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);
		glUniform1i(glGetUniformLocation(gShaderProgram, "skybox"), 0);
	}
	else
	{
		// Pass Colour to Shader.
		glUniform3fv(glGetUniformLocation(gShaderProgram, "skyboxColour"), 1, glm::value_ptr(colour));
	}

	// Draw Skybox.
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	// Restore Depth Func.
	glDepthFunc(GL_LESS);
}

bool Skybox::LoadCubemap()
{
	// Check Number of Faces.
	if (faces.size() != 6)
	{
		logger->Add(new Engine::LoggerMsg("Cubemap", "Not Enough Cubemap Faces!", Engine::LogType::ERROR));
		return false;
	}

	// Generate Texture.
	glGenTextures(1, &cubemapID);

	// Create Textures.
	for (int i = 0; i < faces.size(); i++)
	{
		// Create Dir.
		std::string curDir{ "Data/Cubemaps/" + cubemapDir + "/" + faces[i] };

		// Loop Faces and Check File Types.
		FREE_IMAGE_FORMAT fileFormat{ FreeImage_GetFileType(curDir.c_str()) };

		// Check that the File Format is Supported by FreeImage.
		if (fileFormat == FIF_UNKNOWN)
		{
			logger->Add(new Engine::LoggerMsg("Cubemap", "Unknown File Type!", Engine::LogType::ERROR));
			logger->Add(new Engine::LoggerMsg("Cubemap", "Failed to Load Cubemap Face!", Engine::LogType::ERROR));

			return false;
		}

		// Load Image, Flip it Vertically, and Convert it to 32 Bit.
		FIBITMAP* dib{ FreeImage_Load(fileFormat, curDir.c_str()) };
		FreeImage_FlipVertical(dib);
		dib = FreeImage_ConvertTo32Bits(dib);

		// Check that the Image was Loaded.
		if (!dib)
		{
			logger->Add(new Engine::LoggerMsg("Cubemap", "Failed to Load Cubemap Face Image!", Engine::LogType::ERROR));
			return false;
		}

		// Get Image Size.
		glm::vec2 size{ FreeImage_GetWidth(dib), FreeImage_GetHeight(dib) };
		BYTE* pixels{ FreeImage_GetBits(dib) };

		// Populate Texture.
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, size.x, size.y, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);

		// Unload FreeImage Image.
		FreeImage_Unload(dib);
	}

	// Set Texture Settings.
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	logger->Add(new Engine::LoggerMsg("Cubemap", "Cubemap Loaded.", Engine::LogType::SUCCESS));

	return true;
}

Skybox::Skybox(std::string objectName, std::string cubemapName, SkyboxMode skyboxMode, glm::vec3 skyboxColour) : SceneObject(objectName, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }), cubemapDir(cubemapName), mode(skyboxMode), colour(skyboxColour)
{
	// Set Object Type.
	objectType = SceneObjectType::SKYBOX;

	if (!LoadCubemap())
	{
		logger->Add(new Engine::LoggerMsg("Skybox", "Failed to Load Skybox!", Engine::LogType::ERROR));
		return;
	}

	GLfloat vertices[] =
	{
		// Front face
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,

		// Right face  
		 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,

		 // Back face
		  1.0f, 1.0f,-1.0f,
		 -1.0f, 1.0f,-1.0f,
		 -1.0f,-1.0f,-1.0f,
		  1.0f,-1.0f,-1.0f,

		  // Left face
		  -1.0f, 1.0f,-1.0f,
		  -1.0f, 1.0f, 1.0f,
		  -1.0f,-1.0f, 1.0f,
		  -1.0f,-1.0f,-1.0f,

		  // Top face
		  -1.0f, 1.0f,-1.0f,
		   1.0f, 1.0f,-1.0f,
		   1.0f, 1.0f, 1.0f,
		  -1.0f, 1.0f, 1.0f,

		  // Bottom face
		  -1.0f,-1.0f, 1.0f,
		   1.0f,-1.0f, 1.0f,
		   1.0f,-1.0f,-1.0f,
		  -1.0f,-1.0f,-1.0f,
	};

	GLuint indices[] =
	{
		// Front Face.
		0, 1, 2,
		0, 2, 3,

		// Right Face.
		4, 5, 6,
		4, 6, 7,

		// Back Face.
		8, 9, 10,
		8, 10, 11,

		// Left Face.
		12, 13, 14,
		12, 14, 15,

		// Top Face.
		16, 17, 18,
		16, 18, 19,

		// Bottom Face.
		20, 21, 22,
		20, 22, 23
	};

	// Populate Objects.
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	// Set Vertex Data.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Set Index Data.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Set Vertex Attributes.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // Position.
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	// Initially Populate Buffer.
	strncpy_s(cubemapDirBuffer, cubemapDir.c_str(), sizeof(cubemapDirBuffer));

	// Ensure that the Buffer is Correctly Terminated.
	cubemapDirBuffer[sizeof(cubemapDirBuffer) - 1] = '\0';

	logger->Add(new Engine::LoggerMsg("Skybox", "Skybox Created.", Engine::LogType::SUCCESS));
}

Skybox::~Skybox()
{
	if (cubemapID != 0)
		glDeleteTextures(1, &cubemapID);
}