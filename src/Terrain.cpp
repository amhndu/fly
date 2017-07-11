#include "Terrain.h"
#include "Log.h"
#include "Utility.h"
#include <glm/gtc/matrix_transform.hpp>
#include <simplexnoise.h>
#include <cassert>

namespace fly
{

// Modulo which only returns positive
static int positive_mod(int a, int b)
{
    return (a % b + b) % b;
}

Terrain::Terrain(int radius, int detail) :
            m_radius(radius),
            m_detail(detail),
            m_center(0.f, 0.f),
            m_centerChunk{0, 0},
            m_renderer(radius, detail)
{}

void Terrain::generateChunk(int coord_x, int coord_y, std::vector<float>& heights, std::vector<float>& color)
{
    heights.resize(sq(m_detail + 1 + 2));
    color.resize(sq(m_detail + 1));
    // Generate an extra square outside the chunk to faciliate normal calculations
    for (int i = -1, c = 0; i <= m_detail + 1; ++i)
    {
        for (int j = -1; j <= m_detail + 1; ++j, ++c)
        {
            float x = coord_x + 1.0f * i / m_detail - 0.5f,
                  y = coord_y + 1.0f * j / m_detail - 0.5f;
            float height = 1.5f * std::pow(
                scaled_octave_noise_3d(4.f, 0.15f, 1.f / 6.f, 4.f, 0.f, 1.f, x, y, m_seed), 3.f);
//             height = 0.2;
            heights[c] = height;
            if (i >= 0 && i <= m_detail && j >= 0 && j <= m_detail)
                color[i * (m_detail + 1) + j] =
                scaled_octave_noise_3d(4.f, 0.5f, 1.f / 10.f, 2.f, 0.f, 1.f, x, y, m_seed + 42);
        }
    }
}

void Terrain::updateChunk(int chunk_x, int chunk_y, int coord_x, int coord_y,
                          const std::vector<float>& heights, const std::vector<float>& color)
{
    m_chunkMap[chunk_x + m_radius - 1][chunk_y + m_radius - 1] = {coord_x, coord_y};
    m_renderer.updateChunk(chunk_x, chunk_y, coord_x, coord_y, heights, color);
}

void Terrain::generate(float seed)
{
    m_seed = seed;
    m_renderer.reset(m_radius, m_detail);
    LOG(Info) << "Generating terrain with seed: " << seed << std::endl;
    m_chunkMap.resize(2 * m_radius - 1, std::vector<Pair>(2 * m_radius - 1, {0, 0}));
    std::vector<float> heights, colormap;
    for (int x = -m_radius + 1; x < m_radius; ++x)
    {
        for (int y = -m_radius + 1; y < m_radius; ++y)
        {
            generateChunk(x, y, heights, colormap);
            updateChunk(x, y, x, y, heights, colormap);
        }
    }
    LOG(Info) << "Generated terrain." << std::endl;
}


void Terrain::moveCenter(const glm::vec2& displacement)
{
    m_center += displacement;

    int dx_chunk = static_cast<int>(m_center.x) - m_centerChunk.x;
    int dy_chunk = static_cast<int>(m_center.y) - m_centerChunk.y;
    if (dx_chunk != 0)
    {
        assert(std::abs(dx_chunk) == 1);

        std::vector<float> heights, colormap;
        int chunk_x = positive_mod(m_centerChunk.x + (m_radius - 1) - (m_radius - 1) * dx_chunk,
                              2 * m_radius - 1) - (m_radius  - 1);
        for (int chunk_y = -m_radius + 1; chunk_y < m_radius; ++chunk_y)
        {
            int y = m_chunkMap[chunk_x + m_radius - 1][chunk_y + m_radius - 1].y;
            // dx_chunk is +1 or -1
            generateChunk(m_centerChunk.x + m_radius * dx_chunk, y, heights, colormap);
            updateChunk(chunk_x, chunk_y, m_centerChunk.x + m_radius * dx_chunk, y, heights, colormap);
        }
        m_centerChunk.x +=  dx_chunk;
    }

    if (dy_chunk != 0)
    {
        assert(std::abs(dy_chunk) == 1);

        std::vector<float> heights, colormap;
        int chunk_y = positive_mod(m_centerChunk.y + (m_radius - 1) - (m_radius - 1) * dy_chunk,
                              2 * m_radius - 1) - (m_radius  - 1);
        for (int chunk_x = -m_radius + 1; chunk_x < m_radius; ++chunk_x)
        {
            int x = m_chunkMap[chunk_x + m_radius - 1][chunk_y + m_radius - 1].x;
            generateChunk(x, m_centerChunk.y + m_radius * dy_chunk, heights, colormap);
            updateChunk(chunk_x, chunk_y, x, m_centerChunk.y + m_radius * dy_chunk, heights, colormap);
        }

        m_centerChunk.y +=  dy_chunk;
    }
}

}
