#pragma once
#include "FPSCamera.h"
#include <GLFW/glfw3.h>

FPSCamera c;
bool first = true;
double prevX, prevY;

float delta = 0.0f, last = 0.0f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	double x, y;

	if (first) {
		first = false;
		prevX = xpos;
		prevY = ypos;
	}

	x = xpos - prevX;
	y = prevY - ypos;
	//y = ypos - prevY;
	prevX = xpos;
	prevY = ypos;
	
	c.mouseEvent(x, y);
}

void key_callback(GLFWwindow* window) {

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		c.keyboardEvent(Camera::FORWARD, delta);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		c.keyboardEvent(Camera::BACKWARD, delta);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		c.keyboardEvent(Camera::LEFT, delta);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		c.keyboardEvent(Camera::RIGHT, delta);
}