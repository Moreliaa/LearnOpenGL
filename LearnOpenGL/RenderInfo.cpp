#include "RenderInfo.h"
#include <glad/glad.h>
#include <math.h>

void drawDefault(RenderInfo& info, float time) {
    glUseProgram(info.shaderProgram);
    glBindVertexArray(info.vertexArrayObject);
    //glDrawArrays(GL_TRIANGLES, 0, 3); // use when drawing from the vertex buffer directly
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0); // use when drawing from element buffers
}

void drawUniform(RenderInfo& info, float time) {
    float colorVal = (sin(time) / 2.0f) + 0.5f;
    int vertexColorLocation = glGetUniformLocation(info.shaderProgram, "uniColor");
    glUseProgram(info.shaderProgram);
    glUniform4f(vertexColorLocation, 0.0f, colorVal, 0.0f, 1.0f);
    glBindVertexArray(info.vertexArrayObject);
    //glDrawArrays(GL_TRIANGLES, 0, 3); // use when drawing from the vertex buffer directly
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0); // use when drawing from element buffers
}

void draw(RenderInfo& info, float time) {
    switch (info.renderingMode) {
    case rm_default:
        drawDefault(info, time);
        break;
    case rm_uniform:
        drawUniform(info, time);
        break;
    default:
        break;
    }
}