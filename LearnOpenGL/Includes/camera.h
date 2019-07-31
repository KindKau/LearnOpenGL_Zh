#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <vector>

const float YAW         = -90.0f;
const float PITCH       = 0.0f;
const float SPEED       = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM        = 45.0f;

enum motionInput {
	forward,
	backward,
	right,
	lift
};


class camera
{
public:
	glm::vec3 cameraPos =  glm::vec3(0.0f,0.0f,0.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	float cameraSpeed;
	float cameraSensitivity;
	float camerafov;
	unsigned int Height = 600;
	unsigned int Width = 800;
	float near;
	float far;
	float yaw;
	float pitch;

	float xlast, ylast;

	camera(glm::vec3 inCameraPos, glm::vec3 inCameraFront, glm::vec3 inCameraUp , unsigned int inWidth,unsigned int inHeight)
	{
		cameraPos = inCameraPos;
		cameraFront = normalize(inCameraFront);
		cameraUp = normalize(inCameraUp);
		cameraSpeed = SPEED;
		cameraSensitivity = SENSITIVITY;
		camerafov = ZOOM;
		
		Height = inHeight;
		Width = inWidth;

		near = 0.01f;
		far = 100.0f;

		yaw = YAW;
		pitch = PITCH;
	}

	glm::mat4 getCameraViewMatrix()
	{
		return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}

	glm::mat4 getCameraPerspMatrix()
	{
		return glm::perspective(glm::radians(camerafov), (float)Width / Height, near, far);
	}

	void cameraMotion(motionInput input, float deltTime)
	{
		if (input == forward)
		{
			cameraPos += cameraFront * deltTime * SPEED;
		}
		if (input == backward)
		{
			cameraPos -= cameraFront * deltTime * SPEED;
		}
		if (input == right)
		{
			cameraPos += glm::cross(cameraFront,cameraUp) * deltTime * SPEED;
		}
		if (input == lift)
		{
			cameraPos -= glm::cross(cameraFront, cameraUp) * deltTime * SPEED;
		}
	}

	void cameraZoom(float offset)
	{
		if (camerafov >= 1.0f && camerafov <= 45.0f)
		{
			camerafov -= offset;
		}
		if (camerafov <= 1.0f)
		{
			camerafov = 1.0f;
		}
		if (camerafov >= 45.0f)
		{
			camerafov = 45.0f;
		}
	}

	void cameraRotation(float x, float y)
	{
		float xoffset = x - xlast;
		float yoffset = ylast - y;
		xlast = x;
		ylast = y;

		xoffset *= cameraSensitivity;
		yoffset *= cameraSensitivity;

		yaw += xoffset;
		pitch += yoffset;
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		front.y = sin(glm::radians(pitch));
		front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
		
		cameraFront = normalize(front);
	}
private:


};