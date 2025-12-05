#include "LightWidget.h"

void LightWidget::GUI()
{

}

void LightWidget::Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint gShaderProgram)
{
	if (!active)
		return;

	glUseProgram(gShaderProgram);

	// Create Model Matrix.
	glm::mat4 modelMatrix{ glm::translate(glm::mat4(1.0f), position) };

	// Get Rotation from View Matrix.
	glm::mat4 rotationMatrix{ glm::mat4(glm::mat3(viewMatrix)) };
	rotationMatrix = glm::transpose(rotationMatrix); // Use transpose to get the inverse rotation, as we want the widget to rotate in the opposite direction of the camera.

	// Rotate Widget.
	modelMatrix *= rotationMatrix;

	// Scale Widget.
	modelMatrix = glm::scale(modelMatrix, glm::vec3(size));

	// Upload Matrices to Shader.
	glUniformMatrix4fv(glGetUniformLocation(gShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(gShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(gShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	glBindVertexArray(vao);

	// Pass Widget Colour to Shader.
	glUniform3fv(glGetUniformLocation(gShaderProgram, "baseColor"), 1, glm::value_ptr(colour));

	// Draw.
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

	// Unbind Vertex Array.
	glBindVertexArray(0);
}

LightWidget::LightWidget(bool visible, float widgetSize, WidgetType widgetType) : Widget(visible, widgetSize, widgetType)
{
	// Create Square Vertices.
	vertices =
	{
		{-1.0f, -1.0f, 0.0f}, // Bottom Left.
		{1.0f, -1.0f, 0.0f}, // Bottom Right.
		{1.0f, 1.0f, 0.0f},	// Top Right.
		{-1.0f, 1.0f, 0.0f} // Top Left.
	};

	// Create Square Indices.
	indices =
	{
		0, 1, 2,
		2, 3, 0
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

	std::cout << "[+] Light Widget Created.\n";
}

LightWidget::~LightWidget()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}