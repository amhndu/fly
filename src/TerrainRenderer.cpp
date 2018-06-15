#include "TerrainRenderer.h"
#include "TextureManager.h"
#include "Utility.h"
#include "Log.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cassert>

namespace fly
{


TerrainRenderer::TerrainRenderer(int radius, int detail) :
        m_radius(radius),
        m_detail(detail)
{
    m_vao.bind();

    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glGenBuffers(1, &m_elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer);

    if (m_shaderProgram.loadShaderFile("shaders/terrain.vert", Shader::Vertex))
    {
        LOG(Info) << "Loaded Vertex shader" << std::endl;
    }
    if (m_shaderProgram.loadShaderFile("shaders/terrain.frag", Shader::Fragment))
    {
        LOG(Info) << "Loaded Fragment shader" << std::endl;
    }

    m_vao.unbind();
    ASSERT_GL_ERRORS();
}

TerrainRenderer::~TerrainRenderer()
{
    glDeleteBuffers(1, &m_vertexBuffer);
    glDeleteBuffers(1, &m_elementBuffer);
    ASSERT_GL_ERRORS();
}

void TerrainRenderer::reset(int radius, int detail)
{
    m_radius = radius;
    m_detail = detail;

    m_chunks = sq(2 * radius - 1);
    m_elementsPerChunk = 2 * 3 * sq(detail);
    m_verticesPerChunk = sq(detail + 1);

    LOG(Info) << "Terrain radius: " << m_radius << std::endl;
    LOG(Info) << "Terrain detail: " << m_detail << std::endl;
    LOG(Info) << "Terrain chunks: " << m_chunks << std::endl;
    LOG(Info) << "Elements per chunk: " << m_elementsPerChunk << std::endl;
    LOG(Info) << "Vertices per chunk: " << m_verticesPerChunk << std::endl;

    m_vao.bind();
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_verticesPerChunk * m_chunks,
                 nullptr, GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_elementsPerChunk * m_chunks,
                 nullptr, GL_DYNAMIC_DRAW);

    m_shaderProgram.setAttributeFloat("position", 3, sizeof(Vertex), offsetof(Vertex, position));
    m_shaderProgram.setAttributeFloat("normal"  , 3, sizeof(Vertex), offsetof(Vertex, normal));
    m_shaderProgram.setAttributeFloat("color"   , 1, sizeof(Vertex), offsetof(Vertex, color));

    m_shaderProgram.use();

    m_shaderProgram.setUniform("tex",       TextureManager::getSampler("terrain_lookup"));
    m_shaderProgram.setUniform("ShadowMap", TextureManager::getSampler("ShadowMap"));

    ASSERT_GL_ERRORS();
    m_vao.unbind();
}

void TerrainRenderer::draw()
{
    m_shaderProgram.use();
    rawDraw();
}

void TerrainRenderer::rawDraw()
{
    m_vao.bind();
    glDrawElements(GL_TRIANGLES, m_elementsPerChunk * m_chunks, GL_UNSIGNED_INT, 0);
    m_vao.unbind();
}

void TerrainRenderer::updateChunk(int chunk_x, int chunk_y,
                                  int coord_x, int coord_y,
                                  const std::vector<float>& chunk_heights,
                                  const std::vector<float>& colormap)
{
    std::vector<Vertex> vertices;
    vertices.reserve(m_verticesPerChunk);
    for (int i = 0, c = m_detail + 1 + 2 + 1; i <= m_detail; ++i, c += 2)
    {
        for (int j = 0; j <= m_detail; ++j, ++c)
        {
            float x = coord_x + 1.0f * i / m_detail - 0.5f,
                  y = coord_y + 1.0f * j / m_detail - 0.5f;

            float left  = chunk_heights[c - 1];
            float right = chunk_heights[c + 1];
            float up    = chunk_heights[c - (m_detail + 1 + 2)];
            float down  = chunk_heights[c + (m_detail + 1 + 2)];

            /*
             glm::vec3 diff1 = {1.f / m_detail, 0.f, right - left};
             glm::vec3 diff2 = {0.f, 1.f / m_detail, down  - up};
             auto normal = glm::normalize(glm::cross(diff1, diff2));
             Simplified to:
            */

            auto normal = glm::normalize(glm::vec3{left - right, down - up, 1.f / m_detail});

            GLfloat height = chunk_heights[c];
            vertices.push_back({
                            {x, y, height},
                            {normal.x, normal.y, normal.z},
                            colormap[i * (m_detail + 1) + j]
            });
            assert(vertices.back().color >= 0.f && vertices.back().color <= 1.f);
        }
    }

    auto offset = (chunk_x + m_radius - 1) * (2 * m_radius - 1) + (chunk_y + m_radius - 1);

    std::vector<GLuint> elements;
    elements.reserve(m_elementsPerChunk);
    for (int i = 0, vertex_offset = m_verticesPerChunk * offset; i < m_detail; ++i)
    {
        for (int j = 0; j < m_detail; ++j)
        {
            elements.push_back(vertex_offset + (m_detail + 1) * i + j);
            elements.push_back(vertex_offset + (m_detail + 1) * (i + 1) + j);
            elements.push_back(vertex_offset + (m_detail + 1) * i + j + 1);

            elements.push_back(vertex_offset + (m_detail + 1) * (i + 1) + j + 1);
            elements.push_back(vertex_offset + (m_detail + 1) * i + j + 1);
            elements.push_back(vertex_offset + (m_detail + 1) * (i + 1) + j);
        }
    }

    assert(elements.size() == (unsigned int)m_elementsPerChunk);
    assert(vertices.size() == (unsigned int)m_verticesPerChunk);

    m_vao.bind();
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);      // VAO doesn't store VBO binding
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_verticesPerChunk * offset,
                    sizeof(Vertex) * m_verticesPerChunk, vertices.data());
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_elementsPerChunk * offset,
                    sizeof(GLuint) * m_elementsPerChunk, elements.data());
    ASSERT_GL_ERRORS();
    m_vao.unbind();
}


}
