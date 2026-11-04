#include "World.h"

void World::update(const glm::vec3& playerPos)
{
    int playerChunkX = (int)(std::floor(playerPos.x / Chunk::CHUNK_SIZE));
    int playerChunkZ = (int)(std::floor(playerPos.z / Chunk::CHUNK_SIZE));

    int unload_distance = renderDistance + 2;
    std::vector<glm::ivec2> toRemove;
    for (const auto& [chunkPos, Chunk] : chunks)
    {
        int dx = chunkPos.x - playerChunkX;
        int dz = chunkPos.y - playerChunkZ;
        if (std::abs(dx) > unload_distance || std::abs(dz) > unload_distance) {
            toRemove.push_back(chunkPos); 
        }
    }

    for (size_t i = 0; i < toRemove.size(); ++i) {
        chunks.erase(toRemove[i]);
    }

    std::vector<glm::ivec2> missing;
    for (int dz = -renderDistance; dz <= renderDistance; ++dz) {
        for (int dx = -renderDistance; dx <= renderDistance; ++dx) {
            if (dx * dx + dz * dz > renderDistance * renderDistance) 
                continue;
            
            glm::ivec2 c(playerChunkX + dx, playerChunkZ + dz);
            if (chunks.find(c) == chunks.end())
                missing.push_back(c);
        }
    }

    // sortiraju su po udaljenost da bi se prvo crtali oni koji su najblizi igracu
    std::sort(missing.begin(), missing.end(), [&](const glm::ivec2& a, const glm::ivec2& b) {
        int da = (a.x - playerChunkX) * (a.x - playerChunkX) + (a.y - playerChunkZ) * (a.y - playerChunkZ);
        int db = (b.x - playerChunkX) * (b.x - playerChunkX) + (b.y - playerChunkZ) * (b.y - playerChunkZ);
        return da < db;
    });

    // Mora se ogranici kolko se crtaju jer inace mnogo koci
    const size_t toDraw = std::min(missing.size(), (size_t)maxChunksPerFrame);
    for (size_t i = 0; i < toDraw; ++i) {
        auto chunk = std::make_unique<Chunk>(missing[i].x, missing[i].y);
        chunk->generate(noise);
        chunk->buildMesh();
        chunks.emplace(missing[i], std::move(chunk));
    }
}

void World::draw(const Shader& shader) const
{
    for (const auto& [coord, chunk] : chunks) {
        shader.setMat4("model", glm::translate(glm::mat4(1.0f), chunk->worldOffset()));
        chunk->draw();
    }
}