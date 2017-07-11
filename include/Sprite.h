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
private:
    ShaderProgram m_shader;
    GLuint m_vertexBuffer;
    VertexArrayObject m_vao;
};

}
#endif // SPRITE_H
