#ifndef MODEL_H
#define MODEL_H
#include <glm/glm.hpp>
#include "Shader.h"
#include "VertexArrayObject.h"

namespace fly
{


class Model
{
public:
    Model(const std::string& model_path);
    ~Model();
    void setView(const glm::mat4& view) { m_shaderProgram.setUniform("view", view); }
    void setProjection(const glm::mat4& proj) { m_shaderProgram.setUniform("proj", proj); }
    void setTransform(const glm::mat4& trans) { m_shaderProgram.setUniform("model", trans); }
    void draw();
private:
    ShaderProgram m_shaderProgram;

    uint m_numElements;
    VertexArrayObject m_vao;
    GLuint m_vertexBuffer;
    GLuint m_elementBuffer;

};


}

#endif // MODEL_H
