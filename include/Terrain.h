#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include "TerrainRenderer.h"
#include "Drawable.h"


namespace fly
{


class Terrain : public Drawable
{
public:
    Terrain(int radius, int detail);
    void generate(float seed);
    void draw() override    { m_renderer.draw(); };
    void rawDraw() override { m_renderer.rawDraw(); };
    void moveCenter(const glm::vec2& displacement);
    void setCenter(const glm::vec2& pos) { moveCenter(pos - m_center); }
    void setView(const glm::mat4& view) { m_renderer.setView(view); }
    void setLightSpace(const glm::mat4& lm) { m_renderer.setLightSpace(lm); }
    void setProjection(const glm::mat4& proj) { m_renderer.setProjection(proj); }
private:
    void generateChunk(int coord_x, int coord_y, std::vector<float>& heights,
                       std::vector<float>& color);
    void updateChunk(int chunk_x, int chunk_y, int coord_x, int coord_y,
                     const std::vector<float>& heights, const std::vector<float>& color);

    struct Pair
    {
        int x, y;
    };

    int   m_radius;
    int   m_detail;
    float m_seed;

    glm::vec2 m_center;
    Pair      m_centerChunk;
    std::vector<std::vector<Pair>> m_chunkMap;

    TerrainRenderer m_renderer;
};

}
#endif // TERRAIN_H
