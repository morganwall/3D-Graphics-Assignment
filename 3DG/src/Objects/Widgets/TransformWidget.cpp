#include "TransformWidget.h"

void TransformWidget::GUI()
{

}

void TransformWidget::Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint gShaderProgram)
{
	Widget::Draw(viewMatrix, projectionMatrix, gShaderProgram);

	glBindVertexArray(vao);

	// Loop Through Axis.
	for (int i = 0; i < (sizeof(colours) / sizeof (colours[1])); i++)
	{
		// Draw Axis with it's Colour.
		glUniform3fv(glGetUniformLocation(gShaderProgram, "colour"), 1, glm::value_ptr(colours[i]));
		glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, (void*)(i * 2 * sizeof(unsigned int)));
	}

	// Unbind Vertex Array.
	glBindVertexArray(0);
}

TransformWidget::TransformWidget()
{
	// Create Vertices for Axis Lines.
	vertices = 
	{
		{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, // X.
		{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, // Y.
		{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}  // Z.
	};

	// Create Indices for Axis Lines.
	indices =
	{
		0, 1, // X.
		2, 3, // Y.
		4, 5  // Z.
	};

	// Populate OpenGL Objects.
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(vao);

	// Set Vertex Data.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices.front()), vertices.data(), GL_STATIC_DRAW);

	// Set Index Data.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices.front()), indices.data(), GL_STATIC_DRAW);

	// Set Vertex Attributes.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices.front()), (void*)0); // Position.
	glEnableVertexAttribArray(0);

	// Unbind Vertex Array for Safety.
	glBindVertexArray(0);

	std::cout << "[+] Transform Widget Created.\n";
}

TransformWidget::~TransformWidget()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}