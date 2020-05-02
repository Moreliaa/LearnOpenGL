#pragma once

using namespace std;

enum RenderingMode {
	rm_default, rm_uniform
};

struct RenderInfo
{
	unsigned int shaderProgram;
	unsigned int vertexArrayObject;
	RenderingMode renderingMode = rm_default;
};

void draw(RenderInfo& info, float time);