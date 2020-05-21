#include "Camera.h"

void Camera::updateDirectionVector() {
	direction.x = cos(radians(yaw)) * cos(radians(pitch));
	direction.y = sin(radians(pitch));
	direction.z = sin(radians(yaw)) * cos(radians(pitch));
	direction = normalize(direction);
}

void Camera::moveTo(vec3 newPosition) {
	position = newPosition;
}

void Camera::rotate(float yawDelta, float pitchDelta) {
	yaw += yawDelta;
	pitch += pitchDelta;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	updateDirectionVector();
}

void Camera::strafe(directions dir, float delta) {
	float factor = moveSpeed * delta;
	switch (dir) {
	case directions::d_left:
		position -= normalize(cross(direction, up)) * factor;
		break;
	case directions::d_right:
		position += normalize(cross(direction, up)) * factor;
		break;
	case directions::d_forward:
		position += direction * factor;
		break;
	case directions::d_backward:
		position -= direction * factor;
		break;
	default:
		break;
	}	
}

mat4 Camera::getViewMatrix() {
	return lookAt(position, position + direction, up);
}

Camera::Camera() {
	moveSpeed = 5.0f;
	sensitivity = 0.1f;
	moveTo(vec3(0.0f, 0.0f, 3.0f));
	pitch = 0.0f;
	yaw = -90.0f;
	updateDirectionVector();
	up = vec3(0.0, 1.0, 0.0);
}
