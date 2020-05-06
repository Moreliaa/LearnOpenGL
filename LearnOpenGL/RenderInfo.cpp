#include "RenderInfo.h"
#include <glad/glad.h>
#include <math.h>
#include <iostream>
#include <glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

void drawVertexArray(RenderInfo &info) {
    vec3 cubePositions[] = {
        vec3(0.0f,  0.0f,  0.0f),
        vec3(2.0f,  5.0f, -15.0f),
        vec3(-1.5f, -2.2f, -2.5f),
        vec3(-3.8f, -2.0f, -12.3f),
        vec3(2.4f, -0.4f, -3.5f),
        vec3(-1.7f,  3.0f, -7.5f),
        vec3(1.3f, -2.0f, -2.5f),
        vec3(1.5f,  2.0f, -2.5f),
        vec3(1.5f,  0.2f, -1.5f),
        vec3(-1.3f,  1.0f, -1.5f)
    };

    glBindVertexArray(info.vertexArrayObject);
    for (unsigned int i = 0; i < 10; i++)
    {
        mat4 model(1.0f);
        model = translate(model, cubePositions[i]);
        model = rotate(model, radians(float(20.0f * (i + 1) * glfwGetTime())), vec3(1.0f, 0.3f, 0.5f));
        int varLoc = glGetUniformLocation(info.shaderProgram, "model");
        glUniformMatrix4fv(varLoc, 1, GL_FALSE, glm::value_ptr(model));

        glDrawArrays(GL_TRIANGLES, 0, 36);// use when drawing from the vertex buffer directly
    }
    //glDrawElements(GL_TRIANGLES, info.numIndices, GL_UNSIGNED_INT, 0); // use when drawing from element buffers
}

void drawDefault(RenderInfo& info, float time) {
    glUseProgram(info.shaderProgram);
    drawVertexArray(info);
}

void drawOffset(RenderInfo& info, float time) {
    float offsetVal = (sin(time) / 2.0f) + 0.5f;
    int offsetLocation = glGetUniformLocation(info.shaderProgram, "offset");
    glUseProgram(info.shaderProgram);
    glUniform3f(offsetLocation, offsetVal, 0.0f, 0.0f);
    drawVertexArray(info);
}

void drawTexture(RenderInfo& info, float time) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, info.texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, info.texture2);
    glUseProgram(info.shaderProgram);
    glUniform1i(glGetUniformLocation(info.shaderProgram, "texture1"), 0);
    glUniform1i(glGetUniformLocation(info.shaderProgram, "texture2"), 1);
    drawVertexArray(info);
}

void drawUniform(RenderInfo& info, float time) {
    float colorVal = (sin(time) / 2.0f) + 0.5f;
    int vertexColorLocation = glGetUniformLocation(info.shaderProgram, "uniColor");
    glUseProgram(info.shaderProgram);
    glUniform4f(vertexColorLocation, 0.0f, colorVal, 0.0f, 1.0f);
    drawVertexArray(info);
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