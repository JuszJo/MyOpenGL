#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.h"

// make sure the viewport matches the new window dimensions; note that width and 
// height will be significantly larger than specified on retina displays.
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

GLFWwindow* initWindow(int width, int height) {
    int successInit = glfwInit();

    if(successInit == GLFW_FALSE) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return NULL;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Test", NULL, NULL);

    if (!window) {
        glfwTerminate();
        std::cerr << "Failed to create GLFW window" << std::endl;
        return NULL;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return window;
}

void initGL(GLFWwindow* window) {
    GLenum err = glewInit();

    if(GLEW_OK != err) {
        printf("Error loading Glew");

        glfwDestroyWindow(window);

        glfwTerminate();
    }
}

void mainLoop() {
    
}

int main() {
    int width = 800;
    int height = 600;

    GLFWwindow* window = initWindow(width, height);

    initGL(window);

    Shader myShader("vertexShader.glsl", "fragmentShader.glsl");

    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.3f, 0.4f, 0.6f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT);

        mainLoop();

        processInput(window);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}