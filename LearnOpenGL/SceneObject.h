#pragma once
#include <glad/glad.h>
#include <math.h>
#include <iostream>
#include <glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

enum RenderingMode {
	rm_default, rm_offset, rm_uniform, rm_texture
};

enum VaoType {
	object, light
};

struct SceneObject
{
	vec3 position;
	unsigned int shaderProgram;
	unsigned int vertexArrayObject;
	unsigned int texture1;
	unsigned int texture2;
	unsigned int numIndices;
	RenderingMode renderingMode = rm_default;
	VaoType type = object;
};

mat4 getModelMatrix(SceneObject &obj, float lastFrame);

void draw(SceneObject& info, float time);