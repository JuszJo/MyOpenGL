#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <array>
#include <string>
#include <cstring>
#include <filesystem>

using std::vector;
using std::array;
using std::string;

const char *vertexShaderSource = R"(
    #version 330 core
    
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;

    uniform vec3 npos;
    uniform float scale;

    out vec3 myColor;

    void main() {
        gl_Position = vec4((aPos.x * scale) + npos.x, (aPos.y * scale) + npos.y, aPos.z + npos.z, 1.0);
        // gl_Position = vec4(npos, 1.0);
        myColor = aColor;
    }
)";

const char *fragmentShaderSource = R"(
    #version 330 core

    out vec4 FragColor;

    in vec3 myColor;

    void main() {
        // FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
        FragColor = vec4(myColor, 1.0f);
    }
)";

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

unsigned int initVertexShader() {
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Attach the shader source code to the shader object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

    // compile the shader:
    glCompileShader(vertexShader);

    return vertexShader;
}

unsigned int initFragmentShader() {
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

    glCompileShader(fragmentShader);

    return fragmentShader;
}

void shaderCheckSuccess(unsigned int shader, string type, int success, char infoLog[512]) {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if(!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

void checkLinkSuccess(unsigned int shaderProgram, int success, char infoLog[512]) {
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}

float xOffset = 0.0f;
float yOffset = 0.0f;

void cursor_callback(GLFWwindow* window, double xpos, double ypos) {
    float sens = 1.0f;

    xOffset = (float)((xpos * 2.0f) / 800 - 1.0f) * sens;
    yOffset = (float)(1.0f - (ypos * 2.0f) / 600) * sens;
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "Simple Window", NULL, NULL);

    if (!window) {
        glfwTerminate();
        std::cerr << "Failed to create GLFW window" << std::endl;
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    GLenum err = glewInit();

    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        return 1;
    }

    // Set the viewport size
    int width, height;

    glfwGetFramebufferSize(window, &width, &height);

    // size of rendering window
    glViewport(0, 0, width, height);

    glfwSetCursorPosCallback(window, cursor_callback);

    // SHADERS
    int success = 0;
    char infoLog[512];

    // VERTEX SHADER
    unsigned int vertexShader = initVertexShader();

    shaderCheckSuccess(vertexShader, "VERTEX", success, infoLog);

    // FRAGMENT SHADER
    unsigned int fragmentShader = initFragmentShader();

    shaderCheckSuccess(fragmentShader, "FRAGMENT", success, infoLog);

    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for linking errors
    checkLinkSuccess(shaderProgram, success, infoLog);

    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    /* float vertices[] = {
        -0.5f, -0.5f, 0.0f, 0.3f, 0.5f, 0.7f,
        0.5f, -0.5f, 0.0f,  1.0f, 0.5f, 0.2f,
        0.0f,  0.5f, 0.0f,  0.3f, 0.1f, 0.3f,
    }; */

    // float vertices[] = {
    //     -0.5f, -0.5f, 0.0f,
    //     0.5f, -0.5f, 0.0f, 
    //     -0.5f,  0.5f, 0.0f,
    //     0.5f, 0.5f, 0.0f,
    // };
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 0.3f, 0.5f, 0.7f,
        0.5f, -0.5f, 0.0f,  1.0f, 0.5f, 0.2f,
        -0.5f,  0.5f, 0.0f,  0.3f, 0.1f, 0.3f,
        0.5f, 0.5f, 0.0f, 1.0f, 0.9f, 0.7f
    };

    GLuint indices[] = {
        0, 1, 2,
        1, 2, 3
    };

    GLfloat texCoords[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // postition attribute
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's 
    //bound vertex buffer object so afterwards we can safely unbind

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // double xpos, ypos;

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        // Render here

        // clear
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // glClear(GL_COLOR_BUFFER_BIT);

        // Clear the color buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // printf("xoff: %f, yoff: %f\n", xOffset, yOffset);

        int positionLocation = glGetUniformLocation(shaderProgram, "npos");
        glUniform3f(positionLocation, xOffset, yOffset, 0.0f);

        int scaleLocation = glGetUniformLocation(shaderProgram, "scale");
        glUniform1f(scaleLocation, 0.5f);

        // Draw a triangle
        // glBegin(GL_TRIANGLES);
        // glColor3f(1.0f, 0.0f, 0.0f);
        // glVertex2f(-0.5f, -0.5f);
        // glColor3f(0.0f, 1.0f, 0.0f);
        // glVertex2f(0.5f, -0.5f);
        // glColor3f(0.0f, 0.0f, 1.0f);
        // glVertex2f(0.0f, 0.5f);
        // glEnd();

        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        processInput(window);

        // last
        
        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    // Clean up
    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}