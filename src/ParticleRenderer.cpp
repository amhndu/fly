#include "ParticleRenderer.h"
#include "TextureManager.h"
#include "Log.h"

namespace fly
{

ParticleRenderer::ParticleRenderer(ParticleData& data)
    : m_data(data)
    , m_setFrameBuffer(false)
    , m_mode(Alpha)
{
    static const float vertices[] = {
        -0.5f, 0.f, -0.5f,
         0.5f, 0.f, -0.5f,
        -0.5f, 0.f,  0.5f,
         0.5f, 0.f,  0.5f,
    };

    m_vao.bind();
    glGenBuffers(1, &m_vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &m_positionBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_positionBufferObject);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STREAM_DRAW);

    glGenBuffers(1, &m_colorBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_colorBufferObject);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STREAM_DRAW);

    glGenBuffers(1, &m_sizeBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_sizeBufferObject);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STREAM_DRAW);

    if (m_shader.loadShaderFile("shaders/particle.vert", Shader::Vertex))
        LOG(Info) << "Loaded Vertex Shader" << std::endl;
    if (m_shader.loadShaderFile("shaders/particle.frag", Shader::Fragment))
        LOG(Info) << "Loaded Fragment Shader" << std::endl;
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
    m_shader.setAttributeFloat("vertex", 3, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, m_positionBufferObject);
    m_shader.setAttributeFloat("position", 3, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, m_colorBufferObject);
    m_shader.setAttributeFloat("color", 4, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, m_sizeBufferObject);
    m_shader.setAttributeFloat("size", 1, 0, 0);

    m_shader.setUniform("tex", TextureManager::getSampler("flare"));
}

void ParticleRenderer::useFrameBuffer(FrameBuffer&& fb)
{
    m_frameBuffer = std::move(fb);
    m_setFrameBuffer = true;
    m_sprite.setTexture(TextureManager::getSampler("particlesFrameBuffer"));
}

void ParticleRenderer::setView(const glm::mat4& view)
{
//     glm::mat4 transpose = glm::transpose(view);
//     m_shader.setUniform("CameraRight_worldspace", transpose[0]);
//     m_shader.setUniform("CameraUp_worldspace", transpose[1]);
    m_shader.setUniform("view", view);
    // const glm::vec3 CameraPosition = glm::inverse(view)[3];
}

void ParticleRenderer::draw(FrameBuffer& target)
{
    if (m_setFrameBuffer)
    {
        m_frameBuffer.bind();
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    else
        target.bind();

    if (m_mode == Additive)
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    else
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDepthMask(GL_FALSE);
    m_vao.bind();
    m_shader.use();
    glBindBuffer(GL_ARRAY_BUFFER, m_positionBufferObject); // buffer orphaning for better performance
    glBufferData(GL_ARRAY_BUFFER, m_data.particlesAlive * sizeof(float) * 3, nullptr, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_data.particlesAlive * sizeof(float) * 3, m_data.positionArray.data());

    glBindBuffer(GL_ARRAY_BUFFER, m_colorBufferObject);
    glBufferData(GL_ARRAY_BUFFER, m_data.particlesAlive * sizeof(float) * 4, nullptr, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_data.particlesAlive * sizeof(float) * 4, m_data.colorArray.data());

    glBindBuffer(GL_ARRAY_BUFFER, m_sizeBufferObject);
    glBufferData(GL_ARRAY_BUFFER, m_data.particlesAlive * sizeof(float), nullptr, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_data.particlesAlive * sizeof(float), m_data.sizeArray.data());

    glVertexAttribDivisor(0, 0); // particles vertices: always reuse the same 4 vertices -> 0
    glVertexAttribDivisor(1, 1); // positions: one per each instance -> 1
    glVertexAttribDivisor(2, 1); // color: same as above
    glVertexAttribDivisor(3, 1); // size: same

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, m_data.particlesAlive);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if (m_setFrameBuffer)
    {
        target.bind();
        m_sprite.draw();
    }

    glDepthMask(GL_TRUE);
}

}
