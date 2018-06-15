#include "FrameBuffer.h"
#include "TextureManager.h"
#include "Utility.h"
#include "Log.h"
#include <utility>

namespace fly
{


FrameBuffer::Builder::Builder(unsigned int width, unsigned int height)
    : m_width(width)
    , m_height(height)
    , m_colorAttached(false)
    , m_depthAttached(false)
    , m_depthBuffer(0)
    , m_colorBuffer(0)
{
    glGenFramebuffers(1, &m_frameBuffer);
}

FrameBuffer::Builder& FrameBuffer::Builder::attachDepthTexture(const std::string& textureName)
{
    if (m_depthAttached)
        throw std::invalid_argument("Trying to attach depth buffer but one is already attached");

    m_depthBuffer = TextureManager::generateTexture(textureName);
    glBindTexture(GL_TEXTURE_2D, m_depthBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    ASSERT_GL_ERRORS();
    return attachDepthTexture(m_depthBuffer);
}

FrameBuffer::Builder& FrameBuffer::Builder::attachDepthTexture(GLuint depthBuffer)
{
    if (m_depthAttached)
        throw std::invalid_argument("Trying to attach depth buffer but one is already attached");

    m_depthBuffer = depthBuffer;
    m_depthAttached = true;
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);
    ASSERT_GL_ERRORS();
    return *this;
}

FrameBuffer::Builder& FrameBuffer::Builder::attachColor0(const std::string& textureName)
{
    if (m_colorAttached)
        throw std::invalid_argument("Trying to attach color buffer but one is already attached");

    m_colorBuffer = TextureManager::generateTexture(textureName);
    glBindTexture(GL_TEXTURE_2D, m_colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    ASSERT_GL_ERRORS();
    return attachColor0(m_colorBuffer);
}

FrameBuffer::Builder& FrameBuffer::Builder::attachColor0(GLuint colorBuffer)
{
    if (m_colorAttached)
        throw std::invalid_argument("Trying to attach color buffer but one is already attached");

    m_colorBuffer = colorBuffer;
    m_colorAttached = true;
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBuffer, 0);

    ASSERT_GL_ERRORS();
    return *this;
}

FrameBuffer FrameBuffer::Builder::build()
{
    if (!m_colorAttached)
        glDrawBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error("Framebuffer is incomplete");

    FrameBuffer fbWrapper;
    fbWrapper.m_handle = m_frameBuffer;
    fbWrapper.m_colorBuffer = m_colorBuffer;
    fbWrapper.m_depthBuffer = m_depthBuffer;
    FrameBuffer::bindDefault();
    return fbWrapper;
}


FrameBuffer::FrameBuffer(FrameBuffer &&other)
{
    m_handle      = std::exchange(other.m_handle, 0);
    m_colorBuffer = std::exchange(other.m_colorBuffer, 0);
    m_depthBuffer = std::exchange(other.m_depthBuffer, 0);
}

FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other)
{
    if (m_handle)
    {
        LOG(Debug) << "Copying to a non-empty framebuffer " << m_handle << std::endl;
        glDeleteFramebuffers(1, &m_handle);
        // textures kinda leak..
    }

    m_handle      = std::exchange(other.m_handle, 0);
    m_colorBuffer = std::exchange(other.m_colorBuffer, 0);
    m_depthBuffer = std::exchange(other.m_depthBuffer, 0);
    return *this;
}


FrameBuffer::~FrameBuffer()
{
    if (!m_handle)
        glDeleteFramebuffers(1, &m_handle);
}



}
