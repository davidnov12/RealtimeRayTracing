#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define C_FORWARD front
#define C_BACKWARD -front
#define C_LEFT -right
#define C_RIGHT right

class Camera {

public:

	/**
	* \brief
	*/
	typedef enum {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	} cameraMove;


	/**
	* \brief
	*/
	Camera() {
		this->position = glm::vec3(0.0f, 0.0f, 0.5f);
		this->worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		this->yaw = 0.0f;
		this->pitch = 0.0f;
		this->pitchConstrain = 89.0f;

		updateVectors();
	}


	/**
	* \brief
	*/
	~Camera() {}


	/**
	* \brief
	* \param position
	* \param up
	* \param yaw
	* \param pitch
	*/
	void setValues(glm::vec3 position, glm::vec3 up, float yaw, float pitch) {
		this->position = position;
		this->worldUp = up;
		this->yaw = yaw;
		this->pitch = pitch;
	}


	/**
	* \brief
	* \return
	*/
	glm::vec3 getPosition() {
		return position;
	}


	/**
	* \brief
	* \return
	*/
	glm::mat4 getViewMatrix() {
		return glm::lookAt(position, position + front, up);
	}


	/**
	* \brief
	* \param width
	* \param height
	* \return
	*/
	glm::mat4 getProjectionMatrix(int width, int height) {
		return glm::perspective(glm::radians(zoom), width / (float)height, 0.1f, 100.0f);
	}


	/**
	* \brief
	* \param constrain
	*/
	virtual void setPitchConstrain(float constrain) {
		pitchConstrain = constrain;
	}


	/**
	* \brief
	* \param dir
	* \param delta
	*/
	virtual void keyboardEvent(cameraMove dir, float delta) {}


	/**
	* \brief
	* \param dir
	* \param delta
	*/
	virtual void keyboardEventQuick(int dir, float delta) {}


	/**
	* \brief
	* \param x
	* \param y
	*/
	virtual void mouseEvent(float x, float y) {}


	/**
	* \brief
	* \param x
	* \param y
	*/
	virtual void mouseEventQuick(float x, float y) {}


	/**
	* \brief
	* \param y
	*/
	virtual void scrollEvent(float y) {}


protected:

	const float ZOOM = 45.0f;
	const float SPEED = 0.5f;//2.0f;
	const float SENS = 0.2f;//0.4f;

	glm::vec3 position, worldUp, right, front, up;
	float pitch, yaw, pitchConstrain;
	float zoom = ZOOM;


	/**
	* \brief
	*/
	void updateVectors() {
		glm::vec3 tmp;
		tmp.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		tmp.y = sin(glm::radians(pitch));
		tmp.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		front = glm::normalize(tmp);
		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
	}

};
