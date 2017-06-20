#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include "TerrainRenderer.h"


namespace fly
{


class Terrain
{
public:
    Terrain(ShaderProgram& shader, int radius, int detail) :
            m_radius(radius), m_detail(detail), m_center(0.f, 0.f),
            m_centerChunk{0, 0}, m_deltaChunkMap{0, 0},
            m_renderer(shader, radius, detail) {}
    void generate();
    void draw();
    void moveCenter(const glm::vec2& displacement);
private:
    void generateChunk(int coord_x, int coord_y, std::vector<float>& heights);
    void updateChunk(int chunk_x, int chunk_y, int coord_x, int coord_y, const std::vector<float>& heights);

    struct Pair
    {
        int x, y;
    };

    int m_radius;
    int m_detail;
    glm::vec2 m_center;
    Pair m_centerChunk;
    Pair m_deltaChunkMap;
    std::vector<std::vector<Pair>> m_chunkMap;
    TerrainRenderer m_renderer;
};

}
#endif // TERRAIN_H
