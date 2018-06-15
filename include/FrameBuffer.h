#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <GL/glew.h>
#include <string>

namespace fly
{


class FrameBuffer
{
public:
    FrameBuffer() : m_handle(0), m_depthBuffer(0), m_colorBuffer(0) {};
    FrameBuffer(FrameBuffer&& other);
    FrameBuffer(const FrameBuffer& other) = delete;
    ~FrameBuffer();

    FrameBuffer& operator=(FrameBuffer&& other);

    void bind() {  glBindFramebuffer(GL_FRAMEBUFFER, m_handle); }
    static void bindDefault() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

    GLuint getDepthBuffer() { return m_depthBuffer; }
    GLuint getColorBuffer() { return m_colorBuffer; }

    class Builder;
private:
    GLuint m_handle;
    GLuint m_depthBuffer;
    GLuint m_colorBuffer;
};

class FrameBuffer::Builder
{
public:
    Builder(unsigned int width, unsigned int height);
    // if textureName is non empty, it's mapped in TextureManager
    Builder& attachDepthTexture(const std::string& textureName = "");
    Builder& attachDepthTexture(GLuint depthBuffer);
    Builder& attachColor0(const std::string& textureName = "");
    Builder& attachColor0(GLuint colorBuffer);
    FrameBuffer build();
private:
    const unsigned int m_width, m_height;
    bool m_colorAttached;
    bool m_depthAttached;
    GLuint m_frameBuffer;
    GLuint m_depthBuffer;
    GLuint m_colorBuffer;
};

}

#endif // FRAMEBUFFER_H
