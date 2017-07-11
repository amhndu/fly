#include <glm/gtc/matrix_transform.hpp>
#include "ShadowMap.h"
#include "Utility.h"
#include "TextureManager.h"
#include "Log.h"

namespace fly
{

ShadowMap::ShadowMap(Airplane& plane) :
    m_airplane(plane),
    m_projection(glm::ortho(-0.5f, 0.5f, -0.5f, 0.5f, 0.1f, 50.f))
{
    if (m_shaderProgram.loadShaderFile("shaders/shadow.vert", Shader::Vertex))
    {
        LOG(Info) << "Loaded Vertex shader" << std::endl;
    }
    if (m_shaderProgram.loadShaderFile("shaders/shadow.frag", Shader::Fragment))
    {
        LOG(Info) << "Loaded Fragment shader" << std::endl;
    }

    glGenFramebuffers(1, &m_frameBuffer);

    m_depthMap = TextureManager::generateTexture("ShadowMap");
    glBindTexture(GL_TEXTURE_2D, m_depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 ShadowMap::Width, ShadowMap::Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    ASSERT_GL_ERRORS();
}

ShadowMap::~ShadowMap()
{
    glDeleteFramebuffers(1, &m_frameBuffer);
    // texture is handled by TextureManager
}

const glm::mat4 ShadowMap::update()
{
    auto&& displacement = m_airplane.getPosition();
    glm::mat4 light_view = glm::lookAt(glm::vec3{-0.6f * 4 + displacement.x, 0.3f * 4 + displacement.y, 0.9f * 4},
                                       glm::vec3{0.f + displacement.x, 0.f + displacement.y, 0.f},
                                       glm::vec3{0.0f, 0.0f, 1.0f});
    auto light_space = m_projection * light_view;
    m_shaderProgram.use();

    int viewport_save[4];
    glGetIntegerv(GL_VIEWPORT, viewport_save);
    glViewport(0, 0, ShadowMap::Width, ShadowMap::Height);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);
    // The position attribute assumed to be located at 0
    m_shaderProgram.setUniform("lightSpace", light_space);
    m_shaderProgram.setUniform("model", m_airplane.getModel());
    m_airplane.rawDraw();

    glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(viewport_save[0], viewport_save[1], viewport_save[2], viewport_save[3]);

    return light_space;
}

}
