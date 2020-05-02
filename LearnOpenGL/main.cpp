#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include <vector>
#include "shaderSource.h"
#include "RenderInfo.h"
using namespace std;


int windowWidth_px = 800;
int windowHeight_px = 600;

bool initGlfwSettings() {
    if (!glfwInit()) {
        cout << "Failed to initialize GLFW" << endl;
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    return true;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    windowWidth_px = width;
    windowHeight_px = height;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (key == GLFW_KEY_F3 && action == GLFW_PRESS) { // toggle wireframe mode
        GLint params;
        glGetIntegerv(GL_POLYGON_MODE, &params);
        GLenum newMode = params == GL_FILL ? GL_LINE : GL_FILL;
        glPolygonMode(GL_FRONT_AND_BACK, newMode);
    }
}

GLFWwindow* initGlfwWindow() {
    GLFWwindow* window = glfwCreateWindow(windowWidth_px, windowHeight_px, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
    }
    else {
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetKeyCallback(window, key_callback);
    }
    return window;
}

int cleanUpAllocatedResources(int returnCode) {
    glfwTerminate();
    return returnCode;
}

bool initGlad() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return false;
    }
    return true;
}

unsigned int compileShader(const char *shaderSource, GLenum shaderType) {
    // compile vertex shader
    unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    // log compile errors
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        cout << "Shader compilation failed: " << infoLog << endl;
    }
    return shader;
}

unsigned int createShaderProgram(const char *vtxSource, const char *frgSource) {
    unsigned int shaderProgram = glCreateProgram();
    auto vtxShader = compileShader(vtxSource, GL_VERTEX_SHADER);
    auto frgShader = compileShader(frgSource, GL_FRAGMENT_SHADER);
    glAttachShader(shaderProgram, vtxShader);
    glAttachShader(shaderProgram, frgShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vtxShader);
    glDeleteShader(frgShader);

    // log linking errors
    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "Linking of shader program failed: " << infoLog << endl;
    }
    return shaderProgram;
}

unsigned int createVertexArrayObject(float *vertices, unsigned int verticesSize, unsigned int *indices, unsigned int indicesSize) {
    unsigned int vertexArrayObject; // VAO
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    unsigned int vertexBufferObject; // VBO
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

    unsigned int elementBufferObject; // EBO
    glGenBuffers(1, &elementBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

    // set the vertex attribute pointer to the bound vertex buffer and enable the vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // the VBO is not saved to the VAO, so it is ok to do this first
    glBindVertexArray(0); // the VAO needs to be unbound before unbinding the EBO because the new binding would be saved to the VAO otherwise
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    return vertexArrayObject;
}

void render(GLFWwindow* window, vector<RenderInfo> &renderInfo) {
    float time;

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        time = glfwGetTime();
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);
        for (unsigned int i = 0; i < renderInfo.size(); i++) {
            draw(renderInfo[i], time);
        }
        
        glfwSwapBuffers(window);
    }

    glUseProgram(0);
    glBindVertexArray(0);
}

int main() {
    if (!initGlfwSettings())
        return -1;

    GLFWwindow* window = initGlfwWindow();
    if (window == NULL) {
        return cleanUpAllocatedResources(-1);
    }
    glfwMakeContextCurrent(window);

    if (!initGlad()) {
        return cleanUpAllocatedResources(-1);
    }

    vector<RenderInfo> infoObjects(2);
    float vertices1[] = {
        0.5f,  0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f, 
    };
    float vertices2[] = {
        -0.5f, 0.5f, 0.0f, 
        -0.5f, 0.3f, 0.0f,
        -0.9f, 0.3f, 0.0f,
    };
    unsigned int indices[] = {
        0, 1, 2
    };
    infoObjects[0].vertexArrayObject = createVertexArrayObject(vertices1, sizeof(vertices1), indices, sizeof(indices));
    infoObjects[0].shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource_Uniform);
    infoObjects[0].renderingMode = rm_uniform;
    infoObjects[1].vertexArrayObject = createVertexArrayObject(vertices2, sizeof(vertices2), indices, sizeof(indices));
    infoObjects[1].shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource_Yellow);

    render(window, infoObjects);

    return cleanUpAllocatedResources(0);
}