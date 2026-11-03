#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <memory>

#include "Camera.h"
#include "Shader.h"
#include "Chunk.h"

#include "PerlinNoise.hpp"

namespace {
    int width = 1280;
    int height = 720;

    Camera camera(glm::vec3(0.0f, 150.0f, 3.0f));
    float lastX = width / 2.0f;
    float lastY = height / 2.0f;
    bool firstMouse = true;
}

void framebufferSizeCallback(GLFWwindow*, int w, int h) {
    width = w;
    height = h;
    glViewport(0, 0, w, h);
}

void mouseCallback(GLFWwindow*, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float dx = xpos - lastX;
    float dy = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    camera.rotate(dx, dy);
}

void processInput(GLFWwindow* window, float dt) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.move(CameraMove::Forward, dt);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.move(CameraMove::Backward, dt);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.move(CameraMove::Left, dt);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.move(CameraMove::Right, dt);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera.move(CameraMove::Up, dt);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) camera.move(CameraMove::Down, dt);
}

int main() {
    if (!glfwInit()) {
        std::cerr << "GLFW init nije uspeo\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Procedural Terrain", nullptr, nullptr);
    if (!window) {
        std::cerr << "Prozor nije napravljen\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW init nije uspeo\n";
        return -1;
    }
    std::cout << "OpenGL: " << glGetString(GL_VERSION) << "\n";

    glEnable(GL_DEPTH_TEST);

    Shader shader(std::string(SHADER_DIR) + "basic.vert",
                  std::string(SHADER_DIR) + "basic.frag");

    const siv::PerlinNoise::seed_type seed = 123u;
    const siv::PerlinNoise perlin(seed);

    std::vector<std::unique_ptr<Chunk>> chunks;
    for (int cz = -4; cz < 4; ++cz) {
        for (int cx = -4; cx < 4; ++cx) {
            auto chunk = std::make_unique<Chunk>(cx, cz);
            chunk->generate(perlin);
            chunk->buildMesh();
            chunks.push_back(std::move(chunk));
        }
    }

    camera.speed = 30.0f;
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        float now = (float)glfwGetTime();
        float dt = now - lastFrame;
        lastFrame = now;

        processInput(window, dt);

        glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model(1.0f);
        glm::mat4 view = camera.viewMatrix();
        glm::mat4 projection = glm::perspective(
            glm::radians(60.0f), (float)width / (float)height, 0.1f, 1000.0f);

        shader.use();
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setVec3("lightDir", glm::normalize(glm::vec3(-0.5f, -1.0f, -0.3f)));
        shader.setVec3("objectColor", glm::vec3(0.45f, 0.30f, 0.18f));

        for (const auto& chunk : chunks) {
            shader.setMat4("model", glm::translate(glm::mat4(1.0f), chunk->worldOffset()));
            chunk->draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    chunks.clear(); 
    glfwTerminate();

    return 0;
}