//
// Created by cirin0 on 11.11.24.
//

#ifndef SKYBOX_H
#define SKYBOX_H

#include <glad/glad.h>
#include <vector>
#include "Shader.h"

class Skybox {
public:
    Skybox(float radius, int starCount);

    void draw(const Shader &shader) const;

private:
    GLuint VAO{}, VBO{};
    std::vector<float> vertices;
    int vertexCount{};

    void generateStarField(float radius, int starCount);

    void setupBuffers();
};

#endif //SKYBOX_H
