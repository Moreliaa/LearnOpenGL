const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;

    out vec4 vertexColor;

    void main()
    {
        gl_Position = vec4(aPos, 1.0);
        vertexColor = vec4(0.5, 0.0, 0.0, 1.0);
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

    in vec4 vertexColor;

    void main()
    {
        FragColor = vertexColor;
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