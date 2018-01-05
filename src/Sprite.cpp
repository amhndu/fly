#include "Sprite.h"
#include "Utility.h"
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include "Log.h"

namespace fly
{

Sprite::Sprite()
    : m_recalculateModel(true)
    , m_scale(1.f)
{
    m_vao.bind();
    float vertices[] = {
        -1.0f,  1.0f, 0.f, 1.f, // top-left
        -1.0f, -1.0f, 0.f, 0.f, // bottom-left
         1.0f, -1.0f, 1.f, 0.f, // bottom-right

         1.0f, -1.0f, 1.f, 0.f, // bottom-right
         1.0f,  1.0f, 1.f, 1.f, // top-right
        -1.0f,  1.0f, 0.f, 1.f, // top-left
        };

    const std::string vertex_shader = R"gl(
        #version 330 core
        layout (location = 0) in vec2 position;
        layout (location = 1) in vec2 texcoord;
        out vec2 Texcoord;
        uniform mat4 model;
        void main()
        {
            Texcoord = texcoord;
            gl_Position = model * vec4(position, 0.0, 1.0);
        }
        )gl";
    const std::string fragment_shader = R"gl(
        #version 330 core
        in vec2 Texcoord;
        out vec4 outColor;
        uniform sampler2D tex;
        void main()
        {
            outColor = texture(tex, Texcoord);
        }
        )gl";

    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    if (m_shader.loadShaderString(vertex_shader,   Shader::Vertex))
    {
        LOG(Info) << "Loaded sprite vertex shader" << std::endl;
    }
    if (m_shader.loadShaderString(fragment_shader, Shader::Fragment))
    {
        LOG(Info) << "Loaded sprite fragment shader" << std::endl;
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    m_shader.use();
    m_shader.setAttributeFloat("position", 2, 4 * sizeof(float), 0);
    m_shader.setAttributeFloat("texcoord", 2, 4 * sizeof(float), 2 * sizeof(float));

    m_vao.unbind();
    ASSERT_GL_ERRORS();
}

void Sprite::draw()
{
    if (m_recalculateModel)
    {
        auto model = glm::translate(glm::mat4(1.f), glm::vec3{m_position.x, m_position.y, 0.0f});
        model = glm::scale(model, glm::vec3{m_scale.x, m_scale.y, 1.f});
        m_shader.setUniform("model", model);
        m_recalculateModel = false;
    }
    glDisable(GL_DEPTH_TEST);
    m_shader.use();
    m_vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    m_vao.unbind();
    glEnable(GL_DEPTH_TEST);
}
void Sprite::setTexture(int texture)
{
    LOG(Debug) << "Texture " << texture << " set" << std::endl;
    m_shader.setUniform("tex", texture);
}


}
