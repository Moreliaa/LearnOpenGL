const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    layout (location = 2) in vec2 aTexCoord;

    out vec3 vertexColor;
    out vec2 texCoord;

    uniform mat4 transform = mat4(1.0);

    void main()
    {
        gl_Position = transform * vec4(aPos, 1.0);
        vertexColor = aColor;
        texCoord = aTexCoord;
    }
)";

const char* vertexShaderSource_Offset = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    layout (location = 2) in vec2 aTexCoord;

    out vec3 vertexColor;
    out vec2 texCoord;
    uniform vec3 offset;

    uniform mat4 transform = mat4(1.0);

    void main()
    {
        gl_Position = transform * vec4(aPos + offset, 1.0);
        vertexColor = aColor;
        texCoord = aTexCoord;
    }
)";

const char* fragmentShaderSource_TexCoord = R"(
    #version 330 core
    out vec4 FragColor;

    in vec3 vertexColor;
    in vec2 texCoord;

    uniform sampler2D texture1;
    uniform sampler2D texture2;

    uniform float alphaBalance;

    void main()
    {
        if (texCoord.y <= 1.0)
            FragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), alphaBalance);
        else {
            FragColor = mix(vec4(0.2, 0.3, 0.3, 1.0), texture(texture2, texCoord), alphaBalance);
        }
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