#include "RenderInfo.h"
#include <glad/glad.h>
#include <math.h>
#include <iostream>

using namespace std;

void drawVertexArray(unsigned int vao) {
    glBindVertexArray(vao);
    //glDrawArrays(GL_TRIANGLES, 0, 3); // use when drawing from the vertex buffer directly
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // use when drawing from element buffers
}

void drawDefault(RenderInfo& info, float time) {
    glUseProgram(info.shaderProgram);
    drawVertexArray(info.vertexArrayObject);
}

void drawOffset(RenderInfo& info, float time) {
    float offsetVal = (sin(time) / 2.0f) + 0.5f;
    int offsetLocation = glGetUniformLocation(info.shaderProgram, "offset");
    glUseProgram(info.shaderProgram);
    glUniform3f(offsetLocation, offsetVal, 0.0f, 0.0f);
    drawVertexArray(info.vertexArrayObject);
}

void drawTexture(RenderInfo& info, float time) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, info.texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, info.texture2);
    glUseProgram(info.shaderProgram);
    glUniform1i(glGetUniformLocation(info.shaderProgram, "texture1"), 0);
    glUniform1i(glGetUniformLocation(info.shaderProgram, "texture2"), 1);
    drawVertexArray(info.vertexArrayObject);
}

void drawUniform(RenderInfo& info, float time) {
    float colorVal = (sin(time) / 2.0f) + 0.5f;
    int vertexColorLocation = glGetUniformLocation(info.shaderProgram, "uniColor");
    glUseProgram(info.shaderProgram);
    glUniform4f(vertexColorLocation, 0.0f, colorVal, 0.0f, 1.0f);
    drawVertexArray(info.vertexArrayObject);
}

void draw(RenderInfo& info, float time) {
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