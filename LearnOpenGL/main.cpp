#include "Scene.h"
#include <glfw3.h>
#include <iostream>
#include <vector>
#include "shaderSource.h"
#include "SceneObject.h"
#include "stb_image.h"
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include "Camera.h"

using namespace std;
using namespace glm;


int windowWidth_px = 800;
int windowHeight_px = 600;

float mouse_lastX = NULL;
float mouse_lastY = NULL;
Camera* activeCam = nullptr;

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

    if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        GLint activeShaderProgram;
        glGetIntegerv(GL_CURRENT_PROGRAM, &activeShaderProgram);
        GLint varLoc = glGetUniformLocation(activeShaderProgram, "alphaBalance");
        float val;
        glGetUniformfv(activeShaderProgram, varLoc, &val);
        val += 0.1f;
        val = val < 1.0f ? val : 1.0f;
        glUniform1f(varLoc, val);
    }

    if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        GLint activeShaderProgram;
        glGetIntegerv(GL_CURRENT_PROGRAM, &activeShaderProgram);
        unsigned int varLoc = glGetUniformLocation(activeShaderProgram, "alphaBalance");
        float val;
        glGetUniformfv(activeShaderProgram, varLoc, &val);
        val -= 0.1f;
        val = val > 0.0f ? val : 0.0f;
        glUniform1f(varLoc, val);
    }

    if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        mat4 transform = mat4(1.0); // identity matrix
        // apply transformations - the transformation order should always be scale -> rotation -> translation
        // --> Scalar product: Translation * Rotation * Scale
        transform = translate(transform, vec3(0.2, -0.2, 0.0f));
        transform = rotate(transform, (float)glfwGetTime(), vec3(0.0, 0.0, 1.0));
        transform = scale(transform, vec3(0.5, 0.5, 0.5));

        GLint activeShaderProgram;
        glGetIntegerv(GL_CURRENT_PROGRAM, &activeShaderProgram);
        unsigned int varLoc = glGetUniformLocation(activeShaderProgram, "transform");
        glUniformMatrix4fv(varLoc, 1, GL_FALSE, value_ptr(transform));
    }
    
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (mouse_lastX == NULL || mouse_lastY == NULL) {
        mouse_lastX = xpos;
        mouse_lastY = ypos;
        return;
    }
    if (activeCam == nullptr)
        return;
    float yawDelta = (xpos - mouse_lastX) * activeCam->sensitivity;
    float pitchDelta = (mouse_lastY - ypos) * activeCam->sensitivity;
    mouse_lastX = xpos;
    mouse_lastY = ypos;
    activeCam->rotate(yawDelta, pitchDelta);
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
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window, mouse_callback);
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
    glUseProgram(shaderProgram);
    glUniform1f(glGetUniformLocation(shaderProgram, "alphaBalance"), 0.2);
    glUseProgram(0);
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
    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // the VBO is not saved to the VAO, so it is ok to do this first
    glBindVertexArray(0); // the VAO needs to be unbound before unbinding the EBO because the new binding would be saved to the VAO otherwise
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    return vertexArrayObject;
}

unsigned int createTexture(const char* path, GLenum rgbType, GLenum activeTexture) {
    int texWidth, texHeight, nrChannels;
    unsigned char* texData = stbi_load(path, &texWidth, &texHeight, &nrChannels, 0);
    unsigned int texture = 0;
    if (texData) {
        glGenTextures(1, &texture);
        glActiveTexture(activeTexture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, rgbType, texWidth, texHeight, 0, rgbType, GL_UNSIGNED_BYTE, texData);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else {
        cout << "Failed to load texture" << endl;
    }
    stbi_image_free(texData);
    return texture;
}

void processInputs(GLFWwindow* window, Camera &cam, float delta) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.strafe(directions::d_forward, delta);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.strafe(directions::d_backward, delta);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.strafe(directions::d_left, delta);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.strafe(directions::d_right, delta);
}

void render(GLFWwindow* window, Camera cam, Scene &scene) {
    activeCam = &cam;
    float lastFrame = glfwGetTime();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        float delta = currentFrame - lastFrame;
        lastFrame = glfwGetTime();
        glfwPollEvents();
        processInputs(window, cam, delta);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene.render(cam, lastFrame, windowWidth_px, windowHeight_px);

        
        
        glfwSwapBuffers(window);
    }

    glUseProgram(0);
    glBindVertexArray(0);
    activeCam = nullptr;
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

    stbi_set_flip_vertically_on_load(true);

    float verticesTri[] = {
        // positions        // colors           // textures
        0.0f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.5f, 1.0f,
        0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f
    };
    float verticesRect[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };
    float verticesRectClamp[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // top left 
        -0.5f,  1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 2.0f,   // Clamp top left
         0.5f,  1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 2.0f,   // Clamp top right
    };
    float verticesCube[] = {
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f
    };
    unsigned int indicesTri[] = {
        0, 1, 2
    };
    unsigned int indicesRect[] = {
        0, 1, 3,
        3, 1, 2
    };
    unsigned int indicesRectClamp[] = {
        0, 1, 3,
        3, 1, 2,
        4, 5, 0,
        4, 0, 3
    };

    SceneObject cube;
    cube.vertexArrayObject = createVertexArrayObject(verticesCube, sizeof(verticesCube), indicesRectClamp, sizeof(indicesRectClamp));
    cube.shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource_TexCoord); // textures
    cube.shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource_Light);
    cube.renderingMode = rm_default;
    cube.texture1 = createTexture("../img/container.jpg", GL_RGB, GL_TEXTURE0);
    cube.texture2 = createTexture("../img/awesomeface.png", GL_RGBA, GL_TEXTURE1);
    cube.numIndices = 12;

    glUseProgram(cube.shaderProgram);
    int varLoc = glGetUniformLocation(cube.shaderProgram, "objectColor");
    glUniform3fv(varLoc, 1, value_ptr(vec3(1.0f,0.5f,0.31f)));
    varLoc = glGetUniformLocation(cube.shaderProgram, "lightColor");
    glUniform3fv(varLoc, 1, value_ptr(vec3(1.0f, 1.0f, 1.0f)));
    glUseProgram(0);

    SceneObject lightSource;
    lightSource.vertexArrayObject = createVertexArrayObject(verticesCube, sizeof(verticesCube), indicesRectClamp, sizeof(indicesRectClamp));
    lightSource.shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource_White);
    lightSource.renderingMode = rm_default;
    lightSource.numIndices = 12;
    lightSource.type = light;

    Scene scene = Scene();
    scene.addObject(cube);
    scene.addLightSource(lightSource);
    render(window, Camera(), scene);

    return cleanUpAllocatedResources(0);
}