#include "Chunk.h"

#include <algorithm>
#include <cstddef>
#include <vector>

namespace {

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
};

const glm::ivec3 DIRS[6] = {
    { 1, 0, 0}, 
    {-1, 0, 0}, 
    {0, 1, 0}, 
    {0, -1, 0}, 
    {0, 0, 1}, 
    {0, 0, -1}
};

const glm::vec3 FACE_VERTS[6][4] = {
    {{1,0,1},{1,0,0},{1,1,0},{1,1,1}},  // +X
    {{0,0,0},{0,0,1},{0,1,1},{0,1,0}},  // -X
    {{0,1,1},{1,1,1},{1,1,0},{0,1,0}},  // +Y
    {{0,0,0},{1,0,0},{1,0,1},{0,0,1}},  // -Y
    {{0,0,1},{1,0,1},{1,1,1},{0,1,1}},  // +Z
    {{1,0,0},{0,0,0},{0,1,0},{1,1,0}},  // -Z
};

}

void Chunk::buildMesh()
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    for (int y = 0; y < CHUNK_HEIGHT; ++y) {

        for (int x = 0; x < CHUNK_SIZE; ++x) {
            for (int z = 0; z < CHUNK_SIZE; ++z) {
                BLOCK_TYPE block = getBlock(x, y, z);
                
                if (block == BLOCK_TYPE::AIR)
                    continue;

                for (int face = 0; face < 6; ++face) {
                    glm::ivec3 dir = DIRS[face];
                    int nx = x + dir.x;
                    int ny = y + dir.y;
                    int nz = z + dir.z;

                    if (getBlock(nx, ny, nz) == BLOCK_TYPE::AIR) {
                        GLuint startIndex = vertices.size();
                        
                        for (int i = 0; i < 4; ++i) {
                            glm::vec3 pos = FACE_VERTS[face][i] + glm::vec3(x, y, z);
                            vertices.push_back({pos, glm::vec3(dir)});
                        }

                        indices.push_back(startIndex);
                        indices.push_back(startIndex + 1);
                        indices.push_back(startIndex + 2);
                        indices.push_back(startIndex);
                        indices.push_back(startIndex + 2);
                        indices.push_back(startIndex + 3);
                    }
                }
            }
        }

    }

    if (vao == 0) {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
    }

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    indexCount = static_cast<GLsizei>(indices.size());
}

void Chunk::generate(const siv::PerlinNoise& noise)
{
    for (int x = 0; x < Chunk::CHUNK_SIZE; ++x) {
        for (int z = 0; z < Chunk::CHUNK_SIZE; ++z) {
            //int height = noise.octave2D_01((this->x * Chunk::CHUNK_SIZE + x) * 0.1f, (this->z * Chunk::CHUNK_SIZE + z) * 0.1f, 4, 0.5f) * Chunk::CHUNK_HEIGHT;

            double n = noise.octave2D_01((this->x * CHUNK_SIZE + x) * 0.01, (this->z * CHUNK_SIZE + z) * 0.01, 4, 0.5);
            int height = 40 + (int)(n * 60);

            for (int y = 0; y < Chunk::CHUNK_HEIGHT; ++y) {
                if (y < height) {
                    blocks[index(x, y, z)] = BLOCK_TYPE::DIRT;
                } else {
                    blocks[index(x, y, z)] = BLOCK_TYPE::AIR;
                }
            }
        }
    }
}

void Chunk::draw() const {
    if (indexCount == 0) return;
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}