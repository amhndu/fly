#include <glm/gtc/matrix_transform.hpp>
#include <objload.h>
#include "Model.h"
#include "Log.h"
#include "Utility.h"

namespace fly
{

Model::Model(const std::string& model_path) : m_numElements(0)
{
    m_vao.bind();

    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glGenBuffers(1, &m_elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer);

    if (m_shaderProgram.loadShaderFile("shaders/model.vert", Shader::Vertex))
    {
        LOG(Info) << "Loaded Vertex shader" << std::endl;
    }
    if (m_shaderProgram.loadShaderFile("shaders/model.frag", Shader::Fragment))
    {
        LOG(Info) << "Loaded Fragment shader" << std::endl;
    }

    std::ifstream file (model_path, std::ios_base::in | std::ios_base::binary);

    if (file.is_open() && file.good())
    {
        auto model =  obj::loadModelFromFile(model_path);

        auto& vertices = model.vertex;
        auto& elements = model.faces["default"];

        m_numElements = elements.size();
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]),
                     vertices.data(), GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numElements * sizeof(elements[0]),
                     elements.data(), GL_STATIC_DRAW);

        m_shaderProgram.setAttributeFloat("position", 3, 3 * sizeof(float), 0);
        m_shaderProgram.use();
    }
    else
        LOG(Error) << "Error in opening model file: " << model_path << std::endl;

    m_vao.unbind();
    ASSERT_GL_ERRORS();
}

Model::~Model()
{
    glDeleteBuffers(1, &m_vertexBuffer);
    glDeleteBuffers(1, &m_elementBuffer);
}

void Model::draw()
{
    m_vao.bind();
    m_shaderProgram.use();
    glDrawElements(GL_TRIANGLES, m_numElements, GL_UNSIGNED_SHORT, 0);
    m_vao.unbind();
}



}
