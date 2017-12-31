#ifndef PARTICLERENDERER_H
#define PARTICLERENDERER_H
#include "VertexArrayObject.h"
#include "Shader.h"
#include "ParticleData.h"
#include "FrameBuffer.h"
#include "Sprite.h"

namespace fly
{

enum BlendingMode
{
    Alpha,
    Additive
};

class ParticleRenderer
{
public:
    ParticleRenderer(ParticleData& data);
    void setProjection(const glm::mat4& proj) { m_shader.setUniform("proj", proj); }
    void setView(const glm::mat4& view);
    void draw(FrameBuffer& target);
    void useFrameBuffer(FrameBuffer&& fb);
    void setBlending(BlendingMode mode) { m_mode = mode; }
private:
    GLuint m_vertexBufferObject;
    GLuint m_positionBufferObject;
    GLuint m_colorBufferObject;
    GLuint m_sizeBufferObject;
    VertexArrayObject m_vao;
    ShaderProgram     m_shader;
    ParticleData&     m_data;
    bool              m_setFrameBuffer;
    Sprite            m_sprite;
    FrameBuffer       m_frameBuffer;
    BlendingMode      m_mode;
};

}

#endif // PARTICLERENDERER_H
