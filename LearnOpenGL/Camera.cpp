#include "Camera.h"

void Camera::moveTo(vec3 newPosition) {
	position = newPosition;
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
	moveTo(vec3(0.0f, 0.0f, 3.0f));
	direction = vec3(0.0, 0.0, -1.0);
	up = vec3(0.0, 1.0, 0.0);
}
