#include "Camera.h"

void Camera::MouseMove(glm::vec2 mousePos, GLFWwindow* window, float deltaTime)
{
	// Rotate Camera.
	rotation.p -= mousePos.y * mouseSensitivity;
	rotation.y += mousePos.x * mouseSensitivity;

	// Clamp Angles.
	if (rotation.p > pitchClamp)
		rotation.p = pitchClamp;
	if (rotation.p < -pitchClamp)
		rotation.p = -pitchClamp;

	// Convert Angles to 3D.
	center =
	{
		cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.p)),
		sin(glm::radians(rotation.p)),
		sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.p))
	};
}

void Camera::KeyButtonInput(GLFWwindow* window, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		eye += center * moveSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		eye -= center * moveSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		eye -= glm::normalize(glm::cross(center, up)) * moveSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		eye += glm::normalize(glm::cross(center, up)) * moveSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		eye += up * moveSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
		eye -= up * moveSpeed * deltaTime;
}

void Camera::ScrollInput(float scrollValue)
{
	moveSpeed += scrollValue * moveSpeedSteps;
	if (moveSpeed < 0.0f)
		moveSpeed = 0.0f;
}

void Camera::GUIExtras()
{
	if (!active)
		return;

	ImGui::SeparatorText("Camera Settings");
	ImGui::DragFloat3("Position", &eye.x);
	ImGui::DragFloat3("Rotation", &center.x, 0.1f, -1.0f, 1.0f);
	ImGui::DragFloat("FOV", &fov, 1.0f, 1.0f, 180.0f);
	if (ImGui::DragFloat("Near Clip", &nearClip))
		if (nearClip < 0.1f)
			nearClip = 0.1f;
	if (ImGui::DragFloat("Far Clip", &farClip))
		if (farClip < 0.1f)
			farClip = 0.1f;
	if (ImGui::DragFloat("Mouse Sensitivity", &mouseSensitivity))
		if (mouseSensitivity < 0.1f)
			mouseSensitivity = 0.1f;

	ImGui::SeparatorText("Movement Settings");
	if (ImGui::DragFloat("Speed", &moveSpeed))
		if (moveSpeed < 0.0f)
			moveSpeed = 0.0f;
	if (ImGui::DragFloat("Scroll Speed Steps", &moveSpeedSteps))
		if (moveSpeedSteps < 0.0f)
			moveSpeedSteps = 0.0f;
}

void Camera::Update(float deltaTime)
{

}

Camera::Camera(std::string objectName, glm::vec3 pos, glm::vec3 rot) : SceneObject(objectName, pos, rot)
{
	// Set Object Type.
	objectType = SceneObjectType::CAMERA;

	hasMesh = false;
}