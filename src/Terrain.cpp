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
            m_totalChunks(sq(2 * radius - 1)),
            m_verticesPerChunk(sq(detail + 1)),
            m_center(0.f, 0.f),
            m_centerChunk{0, 0},
            m_renderer(radius, detail)
{}

void Terrain::generateChunk(int chunk_x, int chunk_y, int coord_x, int coord_y,
                            std::vector<float>& heights, std::vector<float>& color)
{
    auto offset = m_verticesPerChunk * ((chunk_x + m_radius - 1) * (2 * m_radius - 1)
                            + (chunk_y + m_radius - 1));
    heights.resize(sq(m_detail + 1 + 2));
    color.resize(m_verticesPerChunk);
    // Generate an extra square outside the chunk to faciliate normal calculations
    for (int i = -1, c = 0, h = 0; i <= m_detail + 1; ++i)
    {
        for (int j = -1; j <= m_detail + 1; ++j, ++c)
        {
            float x = coord_x + 1.0f * i / m_detail - 0.5f,
                  y = coord_y + 1.0f * j / m_detail - 0.5f;
            float height = 1.5f * std::pow(
                scaled_octave_noise_3d(4.f, 0.15f, 1.f / 6.f, 4.f, 0.f, 1.f, x, y, m_seed), 3.f);
            heights[c] = height;
            if (i >= 0 && i <= m_detail && j >= 0 && j <= m_detail)
            {
                color[i * (m_detail + 1) + j] =
                scaled_octave_noise_3d(4.f, 0.5f, 1.f / 10.f, 2.f, 0.f, 1.f, x, y, m_seed + 42);
                m_heightMap[offset + (h++)] = height;
            }
        }
    }
}


void Terrain::generate(float seed)
{
    m_seed = seed;
    m_renderer.reset(m_radius, m_detail);
    LOG(Info) << "Generating terrain with seed: " << seed << std::endl;
    m_chunkMap.resize(2 * m_radius - 1, std::vector<Pair>(2 * m_radius - 1, {0, 0}));
    m_heightMap.resize(m_verticesPerChunk * m_totalChunks);
    std::vector<float> heights, colormap;
    for (int x = -m_radius + 1; x < m_radius; ++x)
    {
        for (int y = -m_radius + 1; y < m_radius; ++y)
        {
            chunk(x, y) = {x, y};
            generateChunk(x, y, x, y, heights, colormap);
            m_renderer.updateChunk(x, y, x, y, heights, colormap);
        }
    }
    LOG(Info) << "Generated terrain." << std::endl;
}

int Terrain::getChunkIndexX(int x)
{
    return positive_mod(x + (m_radius - 1), 2 * m_radius - 1);
}

int Terrain::getChunkIndexY(int y)
{
    return positive_mod(y + (m_radius - 1), 2 * m_radius - 1);
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
        int chunk_x = getChunkIndexX(m_centerChunk.x - (m_radius - 1) * dx_chunk) - (m_radius  - 1);
        for (int chunk_y = -m_radius + 1; chunk_y < m_radius; ++chunk_y)
        {
            auto coord_x = m_centerChunk.x + m_radius * dx_chunk,
                 coord_y = chunk (chunk_x, chunk_y).y;
            // dx_chunk is +1 or -1
            chunk (chunk_x, chunk_y) = {coord_x, coord_y};
            generateChunk(chunk_x, chunk_y, coord_x, coord_y, heights, colormap);
            m_renderer.updateChunk(chunk_x, chunk_y, coord_x, coord_y, heights, colormap);
        }
        m_centerChunk.x +=  dx_chunk;
    }

    if (dy_chunk != 0)
    {
        assert(std::abs(dy_chunk) == 1);

        std::vector<float> heights, colormap;
        int chunk_y = getChunkIndexY(m_centerChunk.y - (m_radius - 1) * dy_chunk) - (m_radius  - 1);
        for (int chunk_x = -m_radius + 1; chunk_x < m_radius; ++chunk_x)
        {
            auto coord_x = chunk (chunk_x, chunk_y).x,
                 coord_y = m_centerChunk.y + m_radius * dy_chunk;
            chunk (chunk_x, chunk_y) = {coord_x, coord_y};
            generateChunk(chunk_x, chunk_y, coord_x, coord_y, heights, colormap);
            m_renderer.updateChunk(chunk_x, chunk_y, coord_x, coord_y, heights, colormap);
        }

        m_centerChunk.y +=  dy_chunk;
    }
}


bool Terrain::above(const OBB& obb)
{
    const auto box         = obb.getAABB();
    const auto bounds_low  = box.position - box.dimensions / 2.f;
    const auto bounds_high = box.position + box.dimensions / 2.f;

    bool collision = false;
//     for (int cx = std::floor(bounds_low.x + 0.5f); !collision
//                 && cx <= static_cast<int>(std::floor(bounds_high.x + 0.5f)); ++cx)
//     {
//         for (int cy = std::floor(bounds_low.y + 0.5f); !collision
//              && cy <= static_cast<int>(std::floor(bounds_high.y + 0.5f)); ++cy)
//         {
//             auto offset = m_verticesPerChunk * (getChunkIndexX(cx) * (2 * m_radius - 1)
//                         + getChunkIndexY(cy));
//             for (int i = 0; i < m_verticesPerChunk; ++i)
//             {
//                 float x = cx + 1.f * (i / (m_detail + 1)) / m_detail - 0.5f,
//                       y = cy + 1.f * (i % (m_detail + 1)) / m_detail - 0.5f;
//                 if (   bounds_low.x < x
//                     && bounds_high.x > x
//                     && bounds_low.y < y
//                     && bounds_high.x > y
//                     && m_heightMap[offset + i] > bounds_low.z)
//                 {
//                     collision = true;
//                     break;
//                 }
//             }
//         }
//     }
//
//     if (!collision)
//         return false;

    collision = false;
    for (int cx = std::floor(bounds_low.x + 0.5f); !collision
                && cx <= static_cast<int>(std::floor(bounds_high.x + 0.5f)); ++cx)
    {
        for (int cy = std::floor(bounds_low.y + 0.5f); !collision
             && cy <= static_cast<int>(std::floor(bounds_high.y + 0.5f)); ++cy)
        {
            auto offset = m_verticesPerChunk * (getChunkIndexX(cx) * (2 * m_radius - 1)
                        + getChunkIndexY(cy));
            for (int i = 0; i + 1 < m_verticesPerChunk - (m_detail + 1); i += 1)
            {
                if ((i + 1) % (m_detail + 1) == 0)
                    continue;

                glm::vec3 a {cx + 1.f * (i / (m_detail + 1)) / m_detail - 0.5f,
                             cy + 1.f * (i % (m_detail + 1)) / m_detail - 0.5f,
                             m_heightMap[offset + i]};

                glm::vec3 b {cx + 1.f * ((i + 1) / (m_detail + 1)) / m_detail - 0.5f,
                             cy + 1.f * ((i + 1) % (m_detail + 1)) / m_detail - 0.5f,
                             m_heightMap[offset + i + 1]};

                glm::vec3 c {cx + 1.f * ((i + m_detail + 1) / (m_detail + 1)) / m_detail - 0.5f,
                             cy + 1.f * ((i + m_detail + 1) % (m_detail + 1)) / m_detail - 0.5f,
                             m_heightMap[offset + i + m_detail + 1]};

                glm::vec3 d {cx + 1.f * ((i + 1 + m_detail + 1) / (m_detail + 1)) / m_detail - 0.5f,
                             cy + 1.f * ((i + 1 + m_detail + 1) % (m_detail + 1)) / m_detail - 0.5f,
                             m_heightMap[offset + i + 1 + m_detail + 1]};


                auto normal1 = glm::cross(c - a, b - a);
                auto normal2 = glm::cross(d - b, d - c);
                assert(normal1.z > 0 && normal2.z > 0);

                for (auto point : obb.points)
                {
                    if (a.x < point.x && c.x > point.x
                     && a.y < point.y && b.y > point.y
                     && (glm::dot(normal1, point - a) < 0
                      || glm::dot(normal2, point - d) < 0))
                    {

                        return true;
                    }
                }
            }
        }
    }

    return false;
}


}
