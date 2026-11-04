#include <cstdint>
#include <functional>
#include <memory>
#include <unordered_map>
#include <glm/glm.hpp>

#include "Chunk.h"
#include "PerlinNoise.hpp"
#include "Shader.h"

struct Vec2Hash {
    size_t operator()(const glm::ivec2& v) const noexcept {
        return std::hash<int64_t>()((int64_t(v.x) << 32) ^ int64_t(uint32_t(v.y)));
    }
};

class World
{
    siv::PerlinNoise noise;
    int renderDistance = 16;
    int maxChunksPerFrame = 2;
    std::unordered_map<glm::ivec2, std::unique_ptr<Chunk>, Vec2Hash> chunks;

public:
    World(siv::PerlinNoise::seed_type seed) : noise(seed) {

    }

    void update(const glm::vec3& playerPos);
    
    void draw(const Shader& shader) const;
    
    void clear() {
        chunks.clear();
    };

};