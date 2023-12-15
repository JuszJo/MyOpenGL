#ifndef  SHADER_H
#define SHADER_H

#include <filesystem>
#include <iostream>
#include <fstream>

class Shader {
    public:
        unsigned int vertexShader, fragmentShader, shaderProgram;

        Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {
            std::filesystem::path vertexPath = vertexShaderPath;
            std::filesystem::path fragmentPath = fragmentShaderPath;

            std::ifstream myStream(vertexPath);

            if (!myStream.is_open()) {
                std::cerr << "Failed to open shader file: " << vertexPath << std::endl;

                return;
            }

            uintmax_t fileSize = std::filesystem::file_size(vertexPath);

            char* data = new char[fileSize + 1];

            myStream.read(data, fileSize);

            data[fileSize] = '\0';

            // delete[] data;

            myStream.close();
        }
};

#endif