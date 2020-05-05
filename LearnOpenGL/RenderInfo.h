#pragma once

using namespace std;

enum RenderingMode {
	rm_default, rm_offset, rm_uniform, rm_texture
};

struct RenderInfo
{
	unsigned int shaderProgram;
	unsigned int vertexArrayObject;
	unsigned int texture1;
	unsigned int texture2;
	unsigned int numIndices;
	RenderingMode renderingMode = rm_default;
};

void draw(RenderInfo& info, float time);