#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Camera.h"
#include "Shader.h"

namespace {
    int width = 1280;
    int height = 720;

    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
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
        lastX = (float)xpos;
        lastY = (float)ypos;
        firstMouse = false;
    }
    float dx = (float)xpos - lastX;
    float dy = lastY - (float)ypos;  // y ide odozdo nagore
    lastX = (float)xpos;
    lastY = (float)ypos;
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

    // Kocka: pozicija (3) + normala (3) po verteksu
    float vertices[] = {
        // zadnja strana
        -0.5f,-0.5f,-0.5f,  0, 0,-1,   0.5f,-0.5f,-0.5f,  0, 0,-1,   0.5f, 0.5f,-0.5f,  0, 0,-1,
         0.5f, 0.5f,-0.5f,  0, 0,-1,  -0.5f, 0.5f,-0.5f,  0, 0,-1,  -0.5f,-0.5f,-0.5f,  0, 0,-1,
        // prednja strana
        -0.5f,-0.5f, 0.5f,  0, 0, 1,   0.5f,-0.5f, 0.5f,  0, 0, 1,   0.5f, 0.5f, 0.5f,  0, 0, 1,
         0.5f, 0.5f, 0.5f,  0, 0, 1,  -0.5f, 0.5f, 0.5f,  0, 0, 1,  -0.5f,-0.5f, 0.5f,  0, 0, 1,
        // leva strana
        -0.5f, 0.5f, 0.5f, -1, 0, 0,  -0.5f, 0.5f,-0.5f, -1, 0, 0,  -0.5f,-0.5f,-0.5f, -1, 0, 0,
        -0.5f,-0.5f,-0.5f, -1, 0, 0,  -0.5f,-0.5f, 0.5f, -1, 0, 0,  -0.5f, 0.5f, 0.5f, -1, 0, 0,
        // desna strana
         0.5f, 0.5f, 0.5f,  1, 0, 0,   0.5f, 0.5f,-0.5f,  1, 0, 0,   0.5f,-0.5f,-0.5f,  1, 0, 0,
         0.5f,-0.5f,-0.5f,  1, 0, 0,   0.5f,-0.5f, 0.5f,  1, 0, 0,   0.5f, 0.5f, 0.5f,  1, 0, 0,
        // donja strana
        -0.5f,-0.5f,-0.5f,  0,-1, 0,   0.5f,-0.5f,-0.5f,  0,-1, 0,   0.5f,-0.5f, 0.5f,  0,-1, 0,
         0.5f,-0.5f, 0.5f,  0,-1, 0,  -0.5f,-0.5f, 0.5f,  0,-1, 0,  -0.5f,-0.5f,-0.5f,  0,-1, 0,
        // gornja strana
        -0.5f, 0.5f,-0.5f,  0, 1, 0,   0.5f, 0.5f,-0.5f,  0, 1, 0,   0.5f, 0.5f, 0.5f,  0, 1, 0,
         0.5f, 0.5f, 0.5f,  0, 1, 0,  -0.5f, 0.5f, 0.5f,  0, 1, 0,  -0.5f, 0.5f,-0.5f,  0, 1, 0,
    };

    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setVec3("lightDir", glm::normalize(glm::vec3(-0.5f, -1.0f, -0.3f)));
        shader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.2f));

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glfwTerminate();
    return 0;
}