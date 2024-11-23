//
// Created by ivang on 26.10.2024.
//

#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include <glad/glad.h>

class Sphere {
public:
    Sphere(float radius, int sectorCount, int stackCount);


    void draw() const;
    void loadTexture(const char* texturePath);
    bool hasTexture;
    GLuint textureID{};

private:
    GLuint VAO{}, VBO{};
    std::vector<float> vertices;
    std::vector<float> texCoords;

    void generateSphere(float radius, int sectorCount, int stackCount);
    void setupBuffers();
};


#endif //SPHERE_H
