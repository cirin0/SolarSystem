//
// Created by ivang on 26.10.2024.
//
#include "Sphere.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <stb_image.h>

Sphere::Sphere(const float radius, const int sectorCount, const int stackCount) : hasTexture(false) {
    generateSphere(radius, sectorCount, stackCount);
    setupBuffers();
}

void Sphere::generateSphere(const float radius, const int sectorCount, const int stackCount) {
    const float sectorStep = 2 * M_PI / sectorCount;
    const float stackStep = M_PI / stackCount;


    for (int i = 0; i < stackCount; ++i) {
        const float stackAngle = M_PI / 2 - i * stackStep;
        const float stackAngleNext = M_PI / 2 - (i + 1) * stackStep;

        for (int j = 0; j < sectorCount; ++j) {
            const float sectorAngle = j * sectorStep;
            const float sectorAngleNext = (j + 1) * sectorStep;

            // Vertex 1
            float x = radius * std::cos(stackAngle) * std::cos(sectorAngle);
            float y = radius * std::cos(stackAngle) * std::sin(sectorAngle);
            float z = radius * std::sin(stackAngle);
            float s = (float) j / sectorCount;
            float t = (float) i / stackCount;
            std::vector<float> vertices_temp = {x, y, z, x / radius, y / radius, z / radius, s, t};
            vertices.insert(vertices.end(), vertices_temp.begin(), vertices_temp.end());

            // Vertex 2
            x = radius * std::cos(stackAngleNext) * std::cos(sectorAngle);
            y = radius * std::cos(stackAngleNext) * std::sin(sectorAngle);
            z = radius * std::sin(stackAngleNext);
            s = (float) j / sectorCount;
            t = (float) (i + 1) / stackCount;
            vertices_temp = {x, y, z, x / radius, y / radius, z / radius, s, t};
            vertices.insert(vertices.end(), vertices_temp.begin(), vertices_temp.end());

            // Vertex 3
            x = radius * std::cos(stackAngle) * std::cos(sectorAngleNext);
            y = radius * std::cos(stackAngle) * std::sin(sectorAngleNext);
            z = radius * std::sin(stackAngle);
            s = (float) (j + 1) / sectorCount;
            t = (float) i / stackCount;
            vertices_temp = {x, y, z, x / radius, y / radius, z / radius, s, t};
            vertices.insert(vertices.end(), vertices_temp.begin(), vertices_temp.end());

            // Vertex 4
            x = radius * std::cos(stackAngleNext) * std::cos(sectorAngleNext);
            y = radius * std::cos(stackAngleNext) * std::sin(sectorAngleNext);
            z = radius * std::sin(stackAngleNext);
            s = (float) (j + 1) / sectorCount;
            t = (float) (i + 1) / stackCount;
            vertices_temp = {x, y, z, x / radius, y / radius, z / radius, s, t};
            vertices.insert(vertices.end(), vertices_temp.begin(), vertices_temp.end());

            // Triangle 1
            vertices.insert(vertices.end(), {
                                vertices[vertices.size() - 32], vertices[vertices.size() - 31],
                                vertices[vertices.size() - 30],
                                vertices[vertices.size() - 29], vertices[vertices.size() - 28],
                                vertices[vertices.size() - 27],
                                vertices[vertices.size() - 26], vertices[vertices.size() - 25]
                            });

            // Triangle 2
            vertices.insert(vertices.end(), {
                                vertices[vertices.size() - 16], vertices[vertices.size() - 15],
                                vertices[vertices.size() - 14],
                                vertices[vertices.size() - 13], vertices[vertices.size() - 12],
                                vertices[vertices.size() - 11],
                                vertices[vertices.size() - 10], vertices[vertices.size() - 9]
                            });
        }
    }
}

void Sphere::setupBuffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Позиція
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    // Нормаль
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Текстурні координати
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Sphere::loadTexture(const char *texturePath) {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = GL_RGB;
        if (nrChannels == 4) format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        hasTexture = true;
    } else {
        std::cerr << "Помилка завантаження текстури: " << texturePath << std::endl;
    }
    stbi_image_free(data);
}


void Sphere::draw() const {
    glBindVertexArray(VAO);
    if (hasTexture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size() / 8);
    glBindVertexArray(0);
}
