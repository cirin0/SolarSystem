#include "Skybox.h"
#include <random>
#include <cmath>

Skybox::Skybox(float radius, int starCount) {
    generateStarField(radius, starCount);
    setupBuffers();
}

void Skybox::generateStarField(float radius, int starCount) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> phiDist(0.0f, 2.0f * M_PI);
    std::uniform_real_distribution<float> thetaDist(0.0f, M_PI);
    std::uniform_real_distribution<float> radiusDist(radius * 0.8f, radius); // Додаємо варіацію відстані

    for (int i = 0; i < starCount; i++) {
        float phi = phiDist(gen);
        float theta = thetaDist(gen);
        float r = radiusDist(gen); // Використовуємо змінний радіус

        // Конвертуємо сферичні координати в декартові
        float x = r * sin(theta) * cos(phi);
        float y = r * sin(theta) * sin(phi);
        float z = r * cos(theta);

        // Додаємо позицію зірки
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
    }

    vertexCount = starCount;
}

void Skybox::setupBuffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Skybox::draw(const Shader& shader) const {
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, vertexCount);
    glBindVertexArray(0);
}//
// Created by cirin0 on 11.11.24.
//
