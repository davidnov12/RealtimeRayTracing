#include "FPSCamera.h"
#include <iostream>

FPSCamera::FPSCamera() {}


FPSCamera::~FPSCamera() {}

std::vector<glm::vec3> FPSCamera::getScreenCoords() {

	std::vector<glm::vec3> scrCoords;

	glm::vec3 tmp = (position - right - up) + (2.0f * front);
	scrCoords.push_back(glm::vec3(tmp.x, tmp.y, tmp.z));

	tmp += 2.0f * up;
	scrCoords.push_back(glm::vec3(tmp.x, tmp.y, tmp.z));

	tmp += (-2.0f * up) + (2.0f * right);
	scrCoords.push_back(glm::vec3(tmp.x, tmp.y, tmp.z));

	tmp += 2.0f * up;
	scrCoords.push_back(glm::vec3(tmp.x, tmp.y, tmp.z));

	return scrCoords;
}


void FPSCamera::keyboardEvent(cameraMove dir, float delta) {

	float distance = SPEED * delta;

	if (dir == FORWARD)
		position += (front * distance);

	else if (dir == BACKWARD)
		position -= (front * distance);

	else if (dir == LEFT)
		position -= (right * distance);

	else if (dir == RIGHT)
		position += (right * distance);

}


void FPSCamera::keyboardEventQuick(int dir, float delta) {
	float distance = delta + delta;
	position += (dir * distance);
}


void FPSCamera::mouseEvent(float x, float y) {

	yaw += (SENS * x);
	pitch += (SENS * y);

	if (pitch > 89.f)
		pitch = 89.f;

	if (pitch < -89.f)
		pitch = -89.f;

	//std::cout << yaw << " " << pitch << std::endl;

	updateVectors();
}


void FPSCamera::mouseEventQuick(float x, float y) {

	if (x != 0)
		yaw += (SENS * x);

	if (y != 0)
		pitch += (SENS * y);

	if (pitch > pitchConstrain)
		pitch = pitchConstrain;

	if (pitch < -pitchConstrain)
		pitch = -pitchConstrain;

	updateVectors();
}


void FPSCamera::scrollEvent(float y) {

	zoom -= y;

	if (zoom < 1.0f)
		zoom = 1.0f;

	if (zoom > 45.0f)
		zoom = 45.0f;
}