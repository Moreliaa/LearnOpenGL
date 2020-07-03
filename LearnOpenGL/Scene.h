#pragma once
#include <glad/glad.h>
#include "SceneObject.h"
#include "Camera.h"
#include <vector>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

class Scene
{
	vector<SceneObject> objects;
	vector<SceneObject> lightSources;


public:
	void addObject(SceneObject& obj);
	void addLightSource(SceneObject& lt);
	void render(Camera& cam, float lastFrame, int width_px, int height_px);
};

