#include "SceneObject.h"
#include <glad/glad.h>
#include <math.h>
#include <iostream>
#include <glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

void drawVertexArray(SceneObject &info) {
    glBindVertexArray(info.vertexArrayObject);
    glDrawArrays(GL_TRIANGLES, 0, 36);// use when drawing from the vertex buffer directly
    //glDrawElements(GL_TRIANGLES, info.numIndices, GL_UNSIGNED_INT, 0); // use when drawing from element buffers
}

void drawDefault(SceneObject& info, float time) {
    drawVertexArray(info);
}

void drawOffset(SceneObject& info, float time) {
    float offsetVal = (sin(time) / 2.0f) + 0.5f;
    int offsetLocation = glGetUniformLocation(info.shaderProgram, "offset");
    glUniform3f(offsetLocation, offsetVal, 0.0f, 0.0f);
    drawVertexArray(info);
}

void drawTexture(SceneObject& info, float time) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, info.texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, info.texture2);
    glUniform1i(glGetUniformLocation(info.shaderProgram, "texture1"), 0);
    glUniform1i(glGetUniformLocation(info.shaderProgram, "texture2"), 1);
    drawVertexArray(info);
}

void drawUniform(SceneObject& info, float time) {
    float colorVal = (sin(time) / 2.0f) + 0.5f;
    int vertexColorLocation = glGetUniformLocation(info.shaderProgram, "uniColor");
    glUniform4f(vertexColorLocation, 0.0f, colorVal, 0.0f, 1.0f);
    drawVertexArray(info);
}

void draw(SceneObject& info, float time) {
    switch (info.renderingMode) {
    case rm_default:
        drawDefault(info, time);
        break;
    case rm_offset:
        drawOffset(info, time);
        break;
    case rm_uniform:
        drawUniform(info, time);
        break;
    case rm_texture:
        drawTexture(info, time);
        break;
    default:
        break;
    }
}