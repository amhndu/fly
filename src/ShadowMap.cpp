#include <glm/gtc/matrix_transform.hpp>
#include "ShadowMap.h"
#include "Utility.h"
#include "TextureManager.h"
#include "Log.h"

namespace fly
{

ShadowMap::ShadowMap(Airplane& plane) :
    m_airplane(plane),
    m_projection(glm::ortho(-0.6f, 0.6f, -0.6f, 0.6f, 0.1f, 100.f)),
    m_frameBuffer(FrameBuffer::Builder(ShadowMap::Width, ShadowMap::Height)
                               .attachDepthTexture("ShadowMap")
                               .build())
{
    if (m_shaderProgram.loadShaderFile("shaders/shadow.vert", Shader::Vertex))
    {
        LOG(Info) << "Loaded Vertex shader" << std::endl;
    }
    if (m_shaderProgram.loadShaderFile("shaders/shadow.frag", Shader::Fragment))
    {
        LOG(Info) << "Loaded Fragment shader" << std::endl;
    }
    ASSERT_GL_ERRORS();
}


const glm::mat4 ShadowMap::update()
{
    auto&& displacement = glm::vec3{m_airplane.getPosition().x, m_airplane.getPosition().y, 0.f};
    glm::mat4 light_view = glm::lookAt(displacement + 2.5f * m_lightDirection,
                                       displacement,
                                       glm::vec3{0.0f, 0.0f, 1.0f});
    auto light_space = m_projection * light_view;
    m_shaderProgram.use();

    glViewport(0, 0, ShadowMap::Width, ShadowMap::Height);
    m_frameBuffer.bind();

    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);

    // The position attribute assumed to be located at 0
    m_shaderProgram.setUniform("lightSpace", light_space);
    m_shaderProgram.setUniform("model", m_airplane.getModel());
    m_airplane.getRenderer().rawDraw();

    glCullFace(GL_BACK);
    return light_space;
}

}
