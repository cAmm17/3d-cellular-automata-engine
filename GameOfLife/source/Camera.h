#pragma once
#pragma once
#include "main.h"

float deltaTime = 0.0f;
float lastFrame = 0.0f;
const float mouseSensitivity = 0.2f;
bool firstMouse = true;

class Camera {
public:
	//vector values for camera position
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;

	//float values for mouse input (looking around)
	float pitch;
	float yaw;
	float lastX;
	float lastY;

	Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up, glm::vec3 right) {
		cameraPos = pos;
		cameraFront = front;
		cameraUp = up;
		cameraRight = right;
		lastX = 400.0f;
		lastY = 300.0f;
	}

	void ProcessKeyboardInput(GLFWwindow *window);
	glm::mat4 setLookAt();
	void ProcessMouseInput(double xpos, double ypos);
};

glm::mat4 Camera::setLookAt() {
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::ProcessKeyboardInput(GLFWwindow *window) {
	float cameraSpeed = 5.0f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}


	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraUp;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraUp;
	}
}

void Camera::ProcessMouseInput(double xpos, double ypos) {
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;

	pitch += yOffset;
	if (pitch > 90.0f) {
		pitch = 89.9f;
	}
	else if (pitch < -90.0f) {
		pitch = -89.9f;
	}

	yaw += xOffset;
	if (yaw > 90.0f) {
		yaw = 89.9f;
	}
	else if (yaw < -90.0f) {
		yaw = -89.9f;
	}

	glm::vec3 camDirection;
	camDirection.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	camDirection.y = sin(glm::radians(pitch));
	camDirection.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(camDirection);
}