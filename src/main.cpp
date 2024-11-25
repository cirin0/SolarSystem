#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Shader.h"
#include "Skybox.h"
#include "Sphere.h"

//#include <direct.h>

// Функція для обробки зміни розміру вікна
void framebuffer_size_callback(GLFWwindow *window, const int width, const int height) {
    glViewport(0, 0, width, height);
}

// Функція для обробки клавіатури
void processInput(GLFWwindow *window, Camera &camera, const float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(RIGHT, deltaTime);
}

// Глобальні змінні для миші
float lastX = 800.0f / 2.0;
float lastY = 600.0f / 2.0;
bool firstMouse = true;

void mouse_callback(GLFWwindow *window, const double xpos, const double ypos) {
    Camera *camera = static_cast<Camera *>(glfwGetWindowUserPointer(window));

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        const float xOffset = xpos - lastX;
        const float yOffset = lastY - ypos; // Інвертуємо y, щоб миша рухалася вгору
        lastX = xpos;
        lastY = ypos;

        // Викликаємо функцію обробки руху миші
        camera->processMouseMovement(xOffset, yOffset);
    } else {
        firstMouse = true;
    }
}


struct PlanetInfo {
    float radius; // Радіус планети
    float distanceFromSun; // Відстань від сонця
    float axialTilt; // Нахил осі в градусах
    float rotationSpeed; // Швидкість обертання навколо своєї осі
    float orbitSpeed; // Швидкість обертання навколо сонця
    bool clockwise; // Напрямок обертання
    glm::vec3 color; // Колір планети якщо текстура відсутня
    const char *texturePath; // Шлях до текстури
};


const float RADIUS_SCALE = 0.0001f; // Зменшений для кращої візуалізації
const float DISTANCE_SCALE = 0.009f; // Зменшений для кращої візуалізації
const float TIME_SCALE = 0.1f; // Для прискорення анімації
const float SUN_ROTATION_SPEED = 5.0f; // Швидкість обертання сонця

const PlanetInfo planets[] = {
    // Радіус, Відстань, Нахил осі, Швидкість обертання, Орбітальна швидкість, Напрямок, Колір, Текстура
    {
        243.7f * RADIUS_SCALE, 80.9f * DISTANCE_SCALE, 0.0f, 1.0f, 4.1f, false,
        glm::vec3(0.8f, 0.8f, 0.8f), "textures/mercury.jpg"
    }, // Меркурій

    {
        605.8f * RADIUS_SCALE, 108.2f * DISTANCE_SCALE, 177.0f, 0.8f, 1.6f, false,
        glm::vec3(0.9f, 0.7f, 0.3f), "textures/venus.jpg"
    }, // Венера

    {
        637.0f * RADIUS_SCALE, 149.6f * DISTANCE_SCALE, 23.5f, 1.0f, 1.0f, false,
        glm::vec3(0.2f, 0.6f, 1.0f), "textures/earth.jpg"
    }, // Земля

    {
        338.5f * RADIUS_SCALE, 227.9f * DISTANCE_SCALE, 25.0f, 1.0f, 0.53f, false,
        glm::vec3(1.0f, 0.3f, 0.3f), "textures/mars.jpg"
    }, // Марс

    {
        3991.0f * RADIUS_SCALE, 478.6f * DISTANCE_SCALE, 33.1f, 3.0f, 0.84f, false,
        glm::vec3(1.0f, 0.9f, 0.6f), "textures/jupiter.jpg"
    }, // Юпітер

    {
        2823.0f * RADIUS_SCALE, 700.5f * DISTANCE_SCALE, 26.7f, 2.2f, 0.34f, false,
        glm::vec3(0.9f, 0.7f, 0.3f), "textures/saturn.jpg"
    }, // Сатурн

    {
        2536.0f * RADIUS_SCALE, 1000.5f * DISTANCE_SCALE, 97.8f, 1.4f, 0.12f, false,
        glm::vec3(0.5f, 0.8f, 0.9f), "textures/uranus.jpg"
    }, // Уран

    {
        2462.0f * RADIUS_SCALE, 1300.1f * DISTANCE_SCALE, 28.3f, 1.5f, 0.06f, false,
        glm::vec3(0.3f, 0.5f, 1.0f), "textures/neptune.jpg"
    }
};

int main() {
    glfwInit();

    // char cwd[256];
    //     if (getcwd(cwd, sizeof(cwd)) != nullptr) {
    //     std::cout << "Поточний каталог: " << cwd << std::endl;
    // }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // Створення вікна
    /* віндовс */
    //GLFWwindow *window = glfwCreateWindow(1600, 900, "Solar System Emulator", nullptr, nullptr);

    /* лінукс */
    GLFWwindow *window = glfwCreateWindow(3000, 1500, "Solar System Emulator", nullptr, nullptr);
    if (!window) {
        std::cerr << "Не вдалося створити вікно GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPosCallback(window, mouse_callback);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Не вдалося ініціалізувати GLAD" << std::endl;
        return -1;
    }

    const Shader shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");

    Sphere sun(0.8f, 50, 50);
    sun.loadTexture("textures/sun.jpg");

    float moonRadius = 173.2f * RADIUS_SCALE;
    Sphere moon(moonRadius, 50, 50);
    moon.loadTexture("textures/moon.jpg");

    std::vector<Sphere> planetSpheres;
    for (const auto &planet: planets) {
        Sphere sphere(planet.radius, 50, 50);
        sphere.loadTexture(planet.texturePath);
        planetSpheres.push_back(sphere);
    }
    Camera camera(glm::vec3(-5.0f, 2.0f, 9.0f), glm::vec3(0.0f, 1.0f, 0.0f), -45.0f, 1.0f);
    glfwSetWindowUserPointer(window, &camera);

    const Shader starShader("shaders/star_vertex_shader.glsl", "shaders/star_fragment_shader.glsl");
    Skybox skybox(8.0f, 20000); // Радіус, Кількість зірок

    std::cout << "Rendering..." << std::endl;
    while (!glfwWindowShouldClose(window)) {
        const size_t PLANETS_COUNT = 8;
        static float lastFrame = 0.0f;
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, camera, deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);

        glEnable(GL_PROGRAM_POINT_SIZE);
        glEnable(GL_BLEND);

        starShader.use();

        glm::mat4 view = camera.getViewMatrix();
        // Створюємо матрицю проекції з кутом огляду 45 градусів, співвідношенням сторін 16:9, ближньою площиною 0.1 і дальньою площиною 100.0
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);


        // рендеринг сонця
        shader.use();
        shader.setMat4("view", glm::value_ptr(view));
        shader.setMat4("projection", glm::value_ptr(projection));

        float currentTime = glfwGetTime() * TIME_SCALE;
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, currentTime * SUN_ROTATION_SPEED, glm::vec3(0.0f, 0.0f, 1.0f)); // обертання сонця
        shader.setVec3("objectColor", 1.0f, 1.0f, 0.0f);
        shader.setMat4("model", glm::value_ptr(model));
        if (sun.hasTexture) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, sun.textureID);
            shader.setInt("ourTexture", 0);
            shader.setBool("hasTexture", true);
        } else {
            shader.setBool("hasTexture", false);
        }
        // відображення сонця
        sun.draw();
        // відображення зірок
        skybox.draw(starShader);
        glDisable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        for (size_t i = 0; i < PLANETS_COUNT; ++i) {
            shader.use();
            shader.setMat4("view", glm::value_ptr(view));
            shader.setMat4("projection", glm::value_ptr(projection));

            model = glm::mat4(1.0f);

            bool stopPlanet = false;

            float orbitAngle = stopPlanet ? 0.0f : currentTime * planets[i].orbitSpeed;
            if (planets[i].clockwise) {
                orbitAngle = -orbitAngle;
            }

            float x = planets[i].distanceFromSun * std::cos(orbitAngle);
            float z = planets[i].distanceFromSun * std::sin(orbitAngle);
            model = glm::translate(model, glm::vec3(x, 0.0f, z));

            model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            // Нахил осі обертання
            model = glm::rotate(model, glm::radians(planets[i].axialTilt), glm::vec3(0.0f, 1.0f, 0.0f));

            // Власне обертання планети
            float rotationAngle = currentTime * planets[i].rotationSpeed;
            if (planets[i].clockwise) {
                rotationAngle = -rotationAngle;
            }
            model = glm::rotate(model, rotationAngle, glm::vec3(0.0f, 0.0f, 1.0f));

            shader.setMat4("model", glm::value_ptr(model));

            if (planetSpheres[i].hasTexture) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, planetSpheres[i].textureID);
                shader.setInt("ourTexture", 0);
                shader.setBool("hasTexture", true);
            } else {
                shader.setBool("hasTexture", false);
                shader.setVec3("objectColor", planets[i].color);
            }
            planetSpheres[i].draw();
        } {
            float moonDistanceFromEarth = 12.0f * DISTANCE_SCALE;
            float moonAxialTilt = 6.7f;
            float moonRotationSpeed = 1.0f;
            float moonOrbitSpeed = 13.0f;

            // Обчислення позиції Землі
            float earthOrbitAngle = currentTime * planets[2].orbitSpeed;
            float earthX = planets[2].distanceFromSun * std::cos(earthOrbitAngle);
            float earthZ = planets[2].distanceFromSun * std::sin(earthOrbitAngle);

            float offsetX = 0.0f; // Зміщення місяця відносно Землі
            //float offsetY = 0.1f;

            bool stopMoon = false;

            float moonOrbitAngle = stopMoon ? 0.0f : currentTime * moonOrbitSpeed;
            float moonX = earthX + moonDistanceFromEarth * std::cos(moonOrbitAngle) + offsetX;
            float moonZ = earthZ + moonDistanceFromEarth * std::sin(moonOrbitAngle);

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(moonX, 0.0f, moonZ));

            model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(moonAxialTilt), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, currentTime * moonRotationSpeed, glm::vec3(0.0f, 0.0f, 1.0f));

            shader.setMat4("model", glm::value_ptr(model));

            if (moon.hasTexture) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, moon.textureID);
                shader.setInt("ourTexture", 0);
                shader.setBool("hasTexture", true);
            } else {
                shader.setBool("hasTexture", false);
                shader.setVec3("objectColor", glm::vec3(0.8f, 0.8f, 0.8f));
            }
            moon.draw();
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
