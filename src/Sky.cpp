#include "Sky.h"
#include "Log.h"
#include "Utility.h"

namespace fly
{

Sky::Sky()
{
    m_vao.bind();

    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);

    if (m_shaderProgram.loadShaderFile("shaders/sky.vert", Shader::Vertex))
    {
        LOG(Info) << "Loaded Vertex shader" << std::endl;
    }
    if (m_shaderProgram.loadShaderFile("shaders/sky.frag", Shader::Fragment))
    {
        LOG(Info) << "Loaded Fragment shader" << std::endl;
    }

    const float vertices[] = {
                -1.0f,  1.0f, -1.0f,
                -1.0f, -1.0f, -1.0f,
                 1.0f, -1.0f, -1.0f,
                 1.0f, -1.0f, -1.0f,
                 1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,

                -1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,

                 1.0f, -1.0f, -1.0f,
                 1.0f, -1.0f,  1.0f,
                 1.0f,  1.0f,  1.0f,
                 1.0f,  1.0f,  1.0f,
                 1.0f,  1.0f, -1.0f,
                 1.0f, -1.0f, -1.0f,

                -1.0f, -1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                 1.0f,  1.0f,  1.0f,
                 1.0f,  1.0f,  1.0f,
                 1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,

                -1.0f,  1.0f, -1.0f,
                 1.0f,  1.0f, -1.0f,
                 1.0f,  1.0f,  1.0f,
                 1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f, -1.0f,

                -1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                 1.0f, -1.0f, -1.0f,
                 1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                 1.0f, -1.0f,  1.0f
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    m_shaderProgram.setAttributeFloat("position", 3, sizeof(float) * 3, 0);

    m_vao.unbind();
    ASSERT_GL_ERRORS();
}

void Sky::draw()
{
    m_shaderProgram.use();
    rawDraw();
}

void Sky::rawDraw()
{
    m_vao.bind();
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
    glDepthFunc(GL_LESS);   // Making the assumption that GL_LESS was the previous config
}

Sky::~Sky()
{
    glDeleteBuffers(1, &m_vertexBuffer);
}
}
