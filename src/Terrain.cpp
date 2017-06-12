#include "Terrain.h"
#include "simplexnoise.h"
#include "Log.h"
#include "Utility.h"
#include "TextureManager.h"
#include <SFML/Graphics/Image.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace fly
{

void Terrain::generate(int m)
{
    // `m' is the level of detail
    // Texture isn't mapped to scale

    m_vertices.reserve((m + 1) * (m + 1));
    for (int i = 0; i <= m; ++i)
    {
        for (int j = 0; j <= m; ++j)
        {
            float x = 2.0f * i / m - 1.f,
                  y = 2.0f * j / m - 1.0f;
            GLfloat height = scaled_octave_noise_3d(4.f, 0.4f, 1.f, 0.f, 0.4f, x, y, 5.14f);
            m_vertices.push_back({{x, height, y},
                                 {static_cast<float>(i % 2), 1 - static_cast<float>(j % 2)}});
        }
    }

    elements.reserve(m * m * 6);
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            elements.push_back((m + 1) * i + j);
            elements.push_back((m + 1) * i + j + 1);
            elements.push_back((m + 1) * (i + 1) + j);

            elements.push_back((m + 1) * (i + 1) + j + 1);
            elements.push_back((m + 1) * i + j + 1);
            elements.push_back((m + 1) * (i + 1) + j);
        }
    }

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(GLint) * elements.size(), elements.data(), GL_STATIC_DRAW);


    m_shader.loadShaderFile("shaders/shader.vert", fly::Shader::Vertex);
    m_shader.loadShaderFile("shaders/shader.frag", fly::Shader::Fragment);
    m_shader.use();
    m_shader.setAttributeFloat("position", 3, sizeof(Vertex), offsetof(Vertex, position));
    m_shader.setAttributeFloat("texcoords", 2, sizeof(Vertex), offsetof(Vertex, texcoords));


    m_shader.setUniform("tex", TextureManager::getSampler("texture.png"));
    m_shader.setUniform("model", glm::mat4(1.f));

    glm::mat4 view = glm::lookAt(
        glm::vec3(1.2f, 1.2f, 1.2f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
        );
    m_shader.setUniform("view", view);

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 10.0f);
    m_shader.setUniform("proj", proj);
    ASSERT_GL_ERRORS();
}

void Terrain::draw()
{
    glDrawElements(GL_TRIANGLES, elements.size(), GL_UNSIGNED_INT, 0);
}

Terrain::~Terrain()
{
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vertexBuffer);

}

}
