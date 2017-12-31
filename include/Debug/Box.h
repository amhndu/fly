#ifndef FLY_BOX_H
#define FLY_BOX_H
#include "Shader.h"
#include "VertexArrayObject.h"

namespace fly
{


class Box
{
public:
    Box();
    void setProjection(const glm::mat4& proj) { m_shader.setUniform("proj", proj); }
    void setTransform(const glm::mat4& transform) { m_shader.setUniform("model", transform); }
    void setView(const glm::mat4& view) { m_shader.setUniform("view", view); }
    void draw();
private:
    ShaderProgram m_shader;
    GLuint m_vertexBuffer;
    VertexArrayObject m_vao;
};

}
#endif // FLY_BOX_H
