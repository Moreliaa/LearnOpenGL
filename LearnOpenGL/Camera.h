#pragma once
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

using namespace glm;

enum class directions {d_left,d_right,d_forward,d_backward};

class Camera
{
	vec3 position;
	vec3 direction;
	vec3 up;
	float yaw;
	float pitch;
	float moveSpeed;

	void updateDirectionVector();

public:
	float sensitivity;

	void moveTo(vec3 newPosition);
	void rotate(float yawDelta, float pitchDelta);
	void strafe(directions dir, float delta);
	mat4 getViewMatrix();

	Camera();

};

