#pragma once
#include "SceneObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera : public SceneObject
{
protected:
	glm::vec3 eye{ 0.0f, 0.0f, 0.0f }; // Camera Position.
	glm::vec3 center{ 0.0f, 0.0f, -1.0f }; // Camera Looking at Position.
	glm::vec3 up{ 0.0f, 1.0f, 0.0f }; // Camera Up Direction.
	glm::vec3 worldUp{ 0.0f, 0.0f, 0.0f }; // World Up Direction.
	glm::vec3 rotation{ 0.0f, 0.0f, 0.0f }; // Roll, Yaw, Pitch.
	float pitchClamp{ 80.0f }; // Min and Max Pitch Clamp.
	float mouseSensitivity{ 0.1f };
	float moveSpeed{ 10.0f };
	float moveSpeedSteps{ 2.5f };
	float fov{ 45.0f };
	float nearClip{ 0.1f };
	float farClip{ 200.0f };

public:
	glm::mat4 GetViewMatrix() { return glm::lookAt(eye, eye + center, up); }
	void MouseMove(glm::vec2 mousePos, GLFWwindow* window, float deltaTime); // Mouse Motion Input.
	void KeyButtonInput(GLFWwindow* window, float deltaTime); // Keyboard and Mouse Button Input.
	void ScrollInput(float scrollValue);

	// Getters.
	glm::vec3 GetEye() { return eye; }
	glm::vec3 GetCenter() { return center; }
	float GetFOV() { return fov; }
	float GetNearClip() { return nearClip; }
	float GetFarClip() { return farClip; }

	// Setters.
	void SetEye(glm::vec3 pos) { eye = pos; }
	void SetCenter(glm::vec3 pos) { center = pos; }
	void SetFOV(float newFOV) { fov = newFOV; }
	void SetNearClip(float clip) { nearClip = clip; }
	void SetFarClip(float clip) { farClip = clip; }

	void GUIExtras() override;
	void Update(float deltaTime) override;

	Camera(std::string objectName = "Camera", glm::vec3 pos = {0.0f, 0.0f, 0.0f}, glm::vec3 rot = {0.0f, 0.0f, 0.0f});
};