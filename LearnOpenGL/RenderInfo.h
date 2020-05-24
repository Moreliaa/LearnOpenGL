#pragma once

using namespace std;

enum RenderingMode {
	rm_default, rm_offset, rm_uniform, rm_texture
};

enum VaoType {
	object, light
};

struct RenderInfo
{
	unsigned int shaderProgram;
	unsigned int vertexArrayObject;
	unsigned int texture1;
	unsigned int texture2;
	unsigned int numIndices;
	RenderingMode renderingMode = rm_default;
	VaoType type = object;
};

void draw(RenderInfo& info, float time);