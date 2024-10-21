#include "Camera.h"
#include <iostream>

vec3 Camera::GetLookVector(CameraDirection Direction) const
{
	switch (Direction)
	{
	case CameraDirection::Up: return UP;
	case CameraDirection::Down: return DOWN;
	case CameraDirection::Left: return LEFT;
	case CameraDirection::Right: return RIGHT;

	case CameraDirection::Front: return FRONT;
	case CameraDirection::Back: return BACK;

	default: return vec3(0);
	}
}

mat4 Camera::Draw(const float aspectRatio)
{
	mat4 projectionMatrix = glm::perspective(PI / 4.0f, aspectRatio, 0.1f, 50.0f);
	mat4 viewMatrix;

	switch (CameraType)
	{
	case CameraType::Free:
		viewMatrix = glm::lookAt(Position, Position + FRONT, UP);

		Direction.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Direction.y = sin(glm::radians(Pitch));
		Direction.z = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));

		projectionMatrix = glm::perspective(glm::radians(FOV), aspectRatio, 0.1f, 100.0f);

		//std::cout << FOV << "\n";

		break;

	case CameraType::Orbit:
		viewMatrix = glm::lookAt(Position, ORBIT_viewportZoom, ORBIT_viewportAngle);
		break;
	}

	// vpMatrix = projectionMatrix * viewMatrix
	return projectionMatrix * viewMatrix;
}