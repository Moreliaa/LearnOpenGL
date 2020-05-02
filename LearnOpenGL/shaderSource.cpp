const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;

    out vec3 vertexColor;

    void main()
    {
        gl_Position = vec4(aPos, 1.0);
        vertexColor = aColor;
    }
)";

const char* vertexShaderSource_Offset = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;

    out vec3 vertexColor;
    uniform vec3 offset;

    void main()
    {
        gl_Position = vec4(aPos + offset, 1.0);
        vertexColor = aColor;
    }
)";

const char* fragmentShaderSource_Orange = R"(
    #version 330 core
    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    }
)";

const char* fragmentShaderSource_Yellow = R"(
    #version 330 core
    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
    }
)";

// use input from the vertex shader to determine the color
const char* fragmentShaderSource_Vtx = R"(
    #version 330 core
    out vec4 FragColor;

    in vec3 vertexColor;

    void main()
    {
        FragColor = vec4(vertexColor, 1.0f);
    }
)";

// use a uniform value saved to the shader program for the color
const char* fragmentShaderSource_Uniform = R"(
    #version 330 core
    out vec4 FragColor;

    uniform vec4 uniColor;

    void main()
    {
        FragColor = uniColor;
    }
)";