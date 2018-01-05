#ifndef SKY_H
#define SKY_H
#include "VertexArrayObject.h"
#include "Shader.h"
#include "Renderer.h"

namespace fly
{

class Sky : public Renderer
{
public:
    Sky();
    ~Sky();
    void draw() override;
    void rawDraw() override;
    void setView(const glm::mat4& view)
    { m_shaderProgram.setUniform("view", glm::mat4(glm::mat3(view))); } // Remove translation
    void setProjection(const glm::mat4& proj) override { m_shaderProgram.setUniform("proj", proj); }
private:
    VertexArrayObject m_vao;
    ShaderProgram m_shaderProgram;

    GLuint m_vertexBuffer;
};

}
#endif // SKY_H
