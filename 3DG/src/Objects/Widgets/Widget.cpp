#include "Widget.h"

void Widget::GUI()
{

}

void Widget::Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint gShaderProgram)
{
	if (!active)
		return;

	glUseProgram(gShaderProgram);

	// Create Model Matrix.
	glm::mat4 modelMatrix{ glm::translate(glm::mat4(1.0f), position) };

	// Scale Object.
	modelMatrix = glm::scale(modelMatrix, glm::vec3(size));

	// Upload Matrices to Shader.
	glUniformMatrix4fv(glGetUniformLocation(gShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(gShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(gShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

Widget::Widget(bool visible, float widgetSize, WidgetType widgetType) : active(visible), size(widgetSize), type(widgetType)
{

}

Widget::~Widget()
{

}