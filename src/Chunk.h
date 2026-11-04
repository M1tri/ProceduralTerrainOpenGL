#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <stdint.h>
#include <array>
#include <memory>

#include "PerlinNoise.hpp"

enum class BLOCK_TYPE : uint8_t {
    AIR,
    DIRT
};

class Chunk {

public:
    constexpr static int CHUNK_SIZE = 16;
    constexpr static int CHUNK_HEIGHT = 256;

private:
    int x;
    int z;

    std::array<BLOCK_TYPE, CHUNK_SIZE * CHUNK_HEIGHT * CHUNK_SIZE> blocks{};

    GLuint vao = 0, vbo = 0, ebo = 0;
    GLsizei indexCount = 0;

public:  
    explicit Chunk(int x, int z) : x(x), z(z) {}
    ~Chunk() = default;

    int index(int x, int y, int z) const {
        return x + CHUNK_SIZE * (y + CHUNK_HEIGHT * z);
    }

    static bool inBounds(int x, int y, int z) {
        return x >= 0 && x < CHUNK_SIZE && y >= 0 && y < CHUNK_HEIGHT && z >= 0 && z < CHUNK_SIZE;
    }

    BLOCK_TYPE getBlock(int x, int y, int z) const {
        return inBounds(x, y, z) ? blocks[index(x, y, z)] : BLOCK_TYPE::AIR;
    }

    glm::vec3 worldOffset() const {
        return glm::vec3(x * CHUNK_SIZE, 0.0f, z * CHUNK_SIZE);
    }

    void generate(const siv::PerlinNoise& noise);
    void buildMesh();
    void draw() const;
};
