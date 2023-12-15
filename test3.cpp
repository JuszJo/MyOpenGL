#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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

void genVertexandBuffers(unsigned int* VAO, unsigned int* VBO, unsigned int* EBO) {
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);
    glGenBuffers(1, EBO);
}

void handleBufferObject(unsigned int VBO, float* vertices, float size) {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
}

void handleVertexObject(unsigned int VAO) {
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
}

void cleanupBuffers() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void mainLoop(unsigned int VAO) {
    glBindVertexArray(VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

int main() {
    int width = 600;
    int height = 600;

    GLFWwindow* window = initWindow(width, height);

    initGL(window);

    Shader myShader("vertexShader.glsl", "fragmentShader.glsl");

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        1, 2, 3
    };

    float verticesSize = sizeof(vertices);

    unsigned int VAO, VBO, EBO, TBO;

    genVertexandBuffers(&VAO, &VBO, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

    handleBufferObject(VBO, vertices, verticesSize);

    handleVertexObject(VAO);

    int textureWidth, textureHeight, numberOfChannels;

    stbi_uc* imageData = stbi_load("wall.jpg", &textureWidth, &textureHeight, &numberOfChannels, 0);

    glGenTextures(1, &TBO);
    glBindTexture(GL_TEXTURE_2D, TBO);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(imageData);

    cleanupBuffers();

    myShader.use();

    glm::mat4 transformationMatrix = glm::mat4(1.0f);

    transformationMatrix = glm::translate(transformationMatrix, glm::vec3(0.3f, 0.0f, 0.0f));

    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.3f, 0.4f, 0.6f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT);

        // glm::mat4 transformationMatrix = glm::mat4(1.0f);

        // transformationMatrix = glm::translate(transformationMatrix, glm::vec3(-0.5f, 0.0f, 0.0f));

        // transformationMatrix = glm::rotate(transformationMatrix, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 1.0f));

        // transformationMatrix = glm::scale(transformationMatrix, glm::vec3(0.3f, 0.3f, 0.3f));

        glUniformMatrix4fv(glGetUniformLocation(myShader.shaderProgram, "transform"), 1, GL_FALSE, glm::value_ptr(transformationMatrix));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBindTexture(GL_TEXTURE_2D, TBO);
        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glm::mat4 transformationMatrix2 = glm::mat4(1.0f);

        // transformationMatrix2 = glm::translate(transformationMatrix2, glm::vec3(0.5f, 0.0f, 0.0f));

        // transformationMatrix2 = glm::rotate(transformationMatrix2, (float)glfwGetTime(), glm::vec3(0.5f, 0.0f, 1.0f));

        // transformationMatrix2 = glm::scale(transformationMatrix2, glm::vec3(0.3f, 0.3f, 0.3f));

        // glUniformMatrix4fv(glGetUniformLocation(myShader.shaderProgram, "transform"), 1, GL_FALSE, glm::value_ptr(transformationMatrix2));

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        processInput(window);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteTextures(1, &TBO);
    glDeleteProgram(myShader.shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
}