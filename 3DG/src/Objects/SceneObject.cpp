#include "SceneObject.h"
#include "Lights/Light.h"

extern Light* light;

void SceneObject::Draw(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint gShaderProgram)
{
	if (!active)
		return;

	// Check if we should Draw as Wireframe.
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUseProgram(gShaderProgram);

	if (light)
		light->ShaderSetup(gShaderProgram);

	// Create Model Matrix.
	glm::mat4 modelMatrix{ glm::translate(glm::mat4(1.0f), position) };

	// Rotate Object.
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	// Scale Object.
	modelMatrix = glm::scale(modelMatrix, scale);

	// Upload Matrices to Shader.
	glUniformMatrix4fv(glGetUniformLocation(gShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(gShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(gShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	glBindVertexArray(vao);
}

void SceneObject::Update(float deltaTime)
{
	// Check if we should Update the Transform Widget.
	if (transformWidget && transformWidget->GetActive())
		transformWidget->SetPosition(position);
}

void SceneObject::GUI()
{

	if (objectType != SceneObjectType::CAMERA)
	{
		ImGui::Checkbox("Active", &active);
		ImGui::SameLine();

		// Create Rename Buffer.
		char nameBuffer[128];

		// Initially Populate Buffer.
		strncpy_s(nameBuffer, name.c_str(), sizeof(nameBuffer));

		// Ensure that the Buffer is Correctly Terminated.
		nameBuffer[sizeof(nameBuffer) - 1] = '\0';

		if (ImGui::InputText("Name", nameBuffer, sizeof(nameBuffer)))
			name = std::string(nameBuffer);

		if (objectType != SceneObjectType::SKYBOX)
		{
			ImGui::DragFloat3("Position", &position.x);
			if (ImGui::DragFloat3("Rotation", &rotation.x)) // This should be rewrote. Quick fix.
			{
				// Make the values wrap back around.
				if (rotation.x > 180.0f)
					rotation.x = -179.0f;
				if (rotation.x < -180.0f)
					rotation.x = 179.0f;

				if (rotation.y > 180.0f)
					rotation.y = -179.0f;
				if (rotation.y < -180.0f)
					rotation.y = 179.0f;

				if (rotation.z > 180.0f)
					rotation.z = -179.0f;
				if (rotation.z < -180.0f)
					rotation.z = 179.0f;
			}
			ImGui::DragFloat3("Scale", &scale.x);
		}
		if (hasMesh)
			ImGui::Checkbox("Show Wireframe", &wireframe);
	}

	GUIExtras();
}

// To be used by child classes to extend GUI functionality.
void SceneObject::GUIExtras()
{

}

void SceneObject::Cleanup()
{
	glDeleteVertexArrays(1, &vao);
}

SceneObject::SceneObject(std::string objectName, glm::vec3 pos, glm::vec3 rot, glm::vec3 objectScale) : name(objectName), position(pos), rotation(rot), scale(objectScale)
{
	// Set the Object Type Again. Not needed, but just in case.
	objectType = SceneObjectType::SCENE_OBJECT;
}

SceneObject::~SceneObject()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &normalBuffer);
}