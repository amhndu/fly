#ifndef SPRITE_H
#define SPRITE_H
#include "Shader.h"
#include "VertexArrayObject.h"

namespace fly
{


class Sprite
{
public:
    Sprite();
    void draw();
    void setTexture(int texture);
    void setPosition(const glm::vec2& pos) { m_position = pos, m_recalculateModel = true; }
    void setScale(const glm::vec2& scale) { m_scale = scale, m_recalculateModel = true; }
private:
    glm::vec2 m_position;
    glm::vec2 m_scale;
    glm::mat4 m_model;
    bool      m_recalculateModel;
    ShaderProgram m_shader;
    GLuint m_vertexBuffer;
    VertexArrayObject m_vao;
};

}
#endif // SPRITE_H
