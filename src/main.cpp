#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
using namespace std;


int windowWidth_px = 800;
int windowHeight_px = 600;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

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

GLFWwindow* createGlfwWindow() {
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

unsigned int createShaderProgram() {
    unsigned int shaderProgram = glCreateProgram();
    auto vtxShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    auto frgShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
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

unsigned int createVertexArrayObject() {
    float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    unsigned int vertexArrayObject; // VAO
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    unsigned int vertexBufferObject; // VBO
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int elementBufferObject; // EBO
    glGenBuffers(1, &elementBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // set the vertex attribute pointer to the bound vertex buffer and enable the vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // the VBO is not saved to the VAO, so it is ok to do this first
    glBindVertexArray(0); // the VAO needs to be unbound before unbinding the EBO because the new binding would be saved to the VAO otherwise
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    return vertexArrayObject;
}

void render(GLFWwindow* window, unsigned int shaderProgram, unsigned int vertexArrayObject) {
    glUseProgram(shaderProgram);
    glBindVertexArray(vertexArrayObject);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);
        //glDrawArrays(GL_TRIANGLES, 0, 3); // use when drawing from the vertex buffer directly
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // use when drawing from element buffers

        glfwSwapBuffers(window);
    }

    glUseProgram(0);
    glBindVertexArray(0);
}

int main() {
    if (!initGlfwSettings())
        return -1;

    GLFWwindow* window = createGlfwWindow();
    if (window == NULL) {
        return cleanUpAllocatedResources(-1);
    }
    glfwMakeContextCurrent(window);

    if (!initGlad()) {
        return cleanUpAllocatedResources(-1);
    }

    auto vao = createVertexArrayObject();
    auto shaderProgram = createShaderProgram();

    render(window, shaderProgram, vao);

    return cleanUpAllocatedResources(0);
}