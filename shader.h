#ifndef SDL2_SHADER_H
#define SDL2_SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    unsigned int ID;

    Shader(const char* vertex_path, const char* fragment_path) {
        std::string vertex_code, fragment_code;
        std::ifstream vertex_file, fragment_file;
        vertex_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragment_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            vertex_file.open(vertex_path);
            std::stringstream vertex_stream;
            vertex_stream << vertex_file.rdbuf();
            vertex_file.close();
            vertex_code = vertex_stream.str();

            fragment_file.open(fragment_path);
            std::stringstream fragment_stream;
            fragment_stream << fragment_file.rdbuf();
            fragment_file.close();
            fragment_code = fragment_stream.str();
        }
        catch (std::ifstream::failure &e) {
            printf("Shader not loaded!");
        }
        const char* vertex_shader_code = vertex_code.c_str();
        const char* fragment_shader_code = fragment_code.c_str();

        unsigned int vertex, fragment;
        int success;
        char info_log[512];

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertex_shader_code, nullptr);
        glCompileShader(vertex);
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex, 512, nullptr, info_log);
            printf("ERROR: Vertex shader: %s", info_log);
        }

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragment_shader_code, nullptr);
        glCompileShader(fragment);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment, 512, nullptr, info_log);
            printf("ERROR: Fragment shader: %s", info_log);
        }

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(ID, 512, nullptr, info_log);
            printf("ERROR: Shader linking: %s", info_log);
        }
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void use() {
        glUseProgram(ID);
    }

    void setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    void setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setMat4(const std::string &name, glm::mat4 value) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1 , GL_FALSE, glm::value_ptr(value));
    }
};

#endif //SDL2_SHADER_H
