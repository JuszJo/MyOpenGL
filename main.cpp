#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <array>
#include <string>
#include <cstring>

using std::vector;
using std::array;
using std::string;

const char *vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    out vec3 myColor;
    void main() {
       gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
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

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 0.3f, 0.5f, 0.7f,
        0.5f, -0.5f, 0.0f,  1.0f, 0.5f, 0.2f,
        0.0f,  0.5f, 0.0f,  0.3f, 0.1f, 0.3f,
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        // Render here

        // clear
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // glClear(GL_COLOR_BUFFER_BIT);

        // Clear the color buffer
        glClear(GL_COLOR_BUFFER_BIT);

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
        glDrawArrays(GL_TRIANGLES, 0, 3);

        processInput(window);

        // last
        
        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Clean up
    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}