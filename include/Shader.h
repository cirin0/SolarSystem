//
// Created by ivang on 26.10.2024.
//

#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glad/glad.h>
#include <glm/vec3.hpp>

class Shader {
public:
    GLuint ID;

    Shader(const char *vertexPath, const char *fragmentPath);

    void use() const {
        glUseProgram(ID);
    }

    void setVec3(const std::string &name, float x, float y, float z) const;

    void setMat4(const std::string &name, const GLfloat *value) const;

    void setVec3(const std::string &name, const glm::vec3 &value) const;

    void setBool(const std::string &name, const bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
    }

    void setInt(const std::string &name, const int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

private:
    static void checkCompileErrors(GLuint shader, const std::string &type);
};

#endif //SHADER_H
