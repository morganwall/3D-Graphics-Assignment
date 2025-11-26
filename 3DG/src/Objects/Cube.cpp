#include "Cube.h"

void Cube::GUIExtras()
{
	ImGui::SeparatorText("Move Between Settings");
	ImGui::Checkbox("Move Between Points", &move);
	ImGui::DragFloat3("Position A", &movePosA.x);
	ImGui::DragFloat3("Position B", &movePosB.x);
	if (ImGui::DragFloat("Move Speed", &moveSpeed))
		if (moveSpeed < 0.0f)
			moveSpeed = 0.0f;
}

void Cube::MoveBetweenPoints(float deltaTime)
{
	// Check that we should Move.
	if (!move)
		return;

	// Get Move Direction.
	glm::vec3 moveDir{ 0.0f, 0.0f, 0.0f };
	if (!moveReversed)
		moveDir = movePosB - position;
	else
		moveDir = movePosA - position;

	// Get Distance to Target.
	float distance{ glm::length(moveDir) };

	// Normalise Move Direction.
	if (distance > 0.0001f)
		moveDir /= distance;

	// Get Move Distance.
	float move{ moveSpeed * deltaTime };

	// Check if we should Reverse Move Direction.
	if (move >= distance)
		moveReversed = !moveReversed;
	else
		position += moveDir * move;
}

void Cube::Update(float deltaTime)
{
	// Call Parent Function.
	SceneObject::Update(deltaTime);

	MoveBetweenPoints(deltaTime);
}

void Cube::Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint gShaderProgram)
{
	SceneObject::Draw(viewMatrix, projectionMatrix, gShaderProgram);

	// Draw Faces with Colours.
	glUniform1i(glGetUniformLocation(gShaderProgram, "hasTexture"), 0); // Tell the shader that we're not using a texture.
	GLint colourLocation{ glGetUniformLocation(gShaderProgram, "baseColour") };
	for (size_t i = 0; i < colours.size(); i++)
	{
		glUniform3fv(colourLocation, 1, glm::value_ptr(colours[i]));
		glDrawElements(GL_TRIANGLES, numIndicesPerFace, GL_UNSIGNED_INT, (void*)(i * numIndicesPerFace * sizeof(GLuint)));
	}

	glBindVertexArray(0); // Unbind Vertex Array.
}

Cube::Cube(std::string objectName, glm::vec3 pos, glm::vec3 rot) : SceneObject(objectName, pos, rot)
{
	// Set Object Type.
	objectType = SceneObjectType::CUBE;

	GLfloat vertices[] =
	{
		// Front face
		-0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f, 0.5f,
		 0.5f,-0.5f, 0.5f,
		-0.5f,-0.5f, 0.5f,

		// Right face  
		 0.5f, 0.5f, 0.5f,
		 0.5f, 0.5f,-0.5f,
		 0.5f,-0.5f,-0.5f,
		 0.5f,-0.5f, 0.5f,

		 // Back face
		  0.5f, 0.5f,-0.5f,
		 -0.5f, 0.5f,-0.5f,
		 -0.5f,-0.5f,-0.5f,
		  0.5f,-0.5f,-0.5f,

		  // Left face
		  -0.5f, 0.5f,-0.5f,
		  -0.5f, 0.5f, 0.5f,
		  -0.5f,-0.5f, 0.5f,
		  -0.5f,-0.5f,-0.5f,

		  // Top face
		  -0.5f, 0.5f,-0.5f,
		   0.5f, 0.5f,-0.5f,
		   0.5f, 0.5f, 0.5f,
		  -0.5f, 0.5f, 0.5f,

		  // Bottom face
		  -0.5f,-0.5f, 0.5f,
		   0.5f,-0.5f, 0.5f,
		   0.5f,-0.5f,-0.5f,
		  -0.5f,-0.5f,-0.5f,
	};

	GLuint indices[] =
	{
		// Front face
		0, 2, 1,
		2, 0, 3,

		// Right face
		4, 6, 5,
		6, 4, 7,

		// Back face
		8, 10, 9,
		10, 8, 11,

		// Left face
		12, 14, 13,
		14, 12, 15,

		// Top face
		16, 18, 17,
		18, 16, 19,

		// Bottom face
		20, 22, 21,
		22, 20, 23
	};

	std::vector<glm::vec3> norms =
	{
		{0.0f, 0.0f, 1.0f}, // Front.
		{1.0f, 0.0f, 0.0f}, // Right.
		{0.0f, 0.0f, -1.0f}, // Back.
		{-1.0f, 0.0f, 0.0f}, // Left.
		{0.0f, 1.0f, 0.0f}, // Top.
		{0.0f, -1.0f, 0.0f} // Bottom.
	};

	colours =
	{
		{1.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f},
		{1.0f, 1.0f, 0.0f},
		{1.0f, 0.0f, 1.0f},
		{0.0f, 1.0f, 1.0f}
	};

	normals.resize(24, { 0.0f, 0.0f, 0.0f });

	unsigned int vertIndex{ 0 };
	// Loop through Faces.
	for (int cubeFace = 0; cubeFace < 6; cubeFace++)
		for (int i = 0; i < 4; i++) // 4 vertices per face.
			normals[vertIndex++] = norms[cubeFace];

	// Populate Objects.
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glGenBuffers(1, &normalBuffer);
	glBindVertexArray(vao);

	// Set Vertex Data.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Set Index Data.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Set Normal Data.
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normals.front()), normals.data(), GL_STATIC_DRAW);

	// Set Vertex Attributes.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // Position.
	glEnableVertexAttribArray(0);

	// Set Normal Attributes.
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // Normals.
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	std::cout << "[+] Cube Created.\n";
}