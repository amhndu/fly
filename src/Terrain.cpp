#include "Terrain.h"
#include "simplexnoise.h"
#include "Log.h"
#include "Utility.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cassert>

namespace fly
{

// Modulo which only returns positive
static int mod_pos(int a, int b)
{
    return (a % b + b) % b;
}

void Terrain::generateChunk(int coord_x, int coord_y, std::vector<float>& heights)
{
    for (int i = 0, c = 0; i <= m_detail; ++i)
    {
        for (int j = 0; j <= m_detail; ++j, ++c)
        {
            float x = coord_x + 1.0f * i / m_detail - 0.5f,
                  y = coord_y + 1.0f * j / m_detail - 0.5f;
            float height = scaled_octave_noise_3d(4.f, 0.2f, 0.3f, 0.f, 0.7f / 0.3f, x, y, m_seed);
            heights[c] = height;
        }
    }
}

void Terrain::updateChunk(int chunk_x, int chunk_y, int coord_x, int coord_y, const std::vector<float>& heights)
{
    m_chunkMap[chunk_x + m_radius - 1][chunk_y + m_radius - 1] = {coord_x, coord_y};
    m_renderer.updateChunk(chunk_x, chunk_y, coord_x, coord_y, 0.15f, 0.15f, heights);
}

void Terrain::generate(float seed)
{
    m_seed = seed;
    m_renderer.reset(m_radius, m_detail);
    m_chunkMap.resize(2 * m_radius - 1, std::vector<Pair>(2 * m_radius - 1, {0, 0}));
    std::vector<float> heights(sq(m_detail + 1));
    for (int x = -m_radius + 1; x < m_radius; ++x)
    {
        for (int y = -m_radius + 1; y < m_radius; ++y)
        {
            generateChunk(x, y, heights);
            updateChunk(x, y, x, y, heights);
        }
    }
}

void Terrain::draw()
{
    m_renderer.draw();
}

void Terrain::moveCenter(const glm::vec2& displacement)
{
    // TODO Make it so that crossing a chunk boundary back and forth doesn't cause thrashing

    m_center += displacement;

    int dx_chunk = static_cast<int>(m_center.x) - m_centerChunk.x;
    int dy_chunk = static_cast<int>(m_center.y) - m_centerChunk.y;
    if (dx_chunk != 0)
    {
        assert(std::abs(dx_chunk) == 1);

        std::vector<float> heights(sq(m_detail + 1));
        int chunk_x = mod_pos(m_centerChunk.x + (m_radius - 1) - (m_radius - 1) * dx_chunk,
                              2 * m_radius - 1) - (m_radius  - 1);
        for (int chunk_y = -m_radius + 1; chunk_y < m_radius; ++chunk_y)
        {
            int y = m_chunkMap[chunk_x + m_radius - 1][chunk_y + m_radius - 1].y;
            // dx_chunk is +1 or -1
            generateChunk(m_centerChunk.x + m_radius * dx_chunk, y, heights);
            updateChunk(chunk_x, chunk_y, m_centerChunk.x + m_radius * dx_chunk, y, heights);
        }
        m_centerChunk.x +=  dx_chunk;
    }

    if (dy_chunk != 0)
    {
        assert(std::abs(dy_chunk) == 1);

        std::vector<float> heights(sq(m_detail + 1));
        int chunk_y = mod_pos(m_centerChunk.y + (m_radius - 1) - (m_radius - 1) * dy_chunk,
                              2 * m_radius - 1) - (m_radius  - 1);
        for (int chunk_x = -m_radius + 1; chunk_x < m_radius; ++chunk_x)
        {
            int x = m_chunkMap[chunk_x + m_radius - 1][chunk_y + m_radius - 1].x;
            generateChunk(x, m_centerChunk.y + m_radius * dy_chunk, heights);
            updateChunk(chunk_x, chunk_y, x, m_centerChunk.y + m_radius * dy_chunk, heights);
        }

        m_centerChunk.y +=  dy_chunk;
    }
}

}
