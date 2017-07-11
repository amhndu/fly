#include "Sprite.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Utility.h"

namespace fly
{

Sprite::Sprite()
{
    m_vao.bind();
    float vertices[] = {
        -0.5f,  0.5f, 0.f, 0.f, // top-left
        -0.5f, -0.5f, 0.f, 1.f, // bottom-left
         0.5f, -0.5f, 1.f, 1.f, // bottom-right

         0.5f, -0.5f, 1.f, 1.f, // bottom-right
         0.5f,  0.5f, 1.f, 0.f, // top-right
        -0.5f,  0.5f, 0.f, 0.f, // top-left
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
            outColor = vec4(vec3(texture(tex, Texcoord)), 1.0);
        }
        )gl";

    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    assert(m_shader.loadShaderString(vertex_shader,   fly::Shader::Vertex));
    assert(m_shader.loadShaderString(fragment_shader, fly::Shader::Fragment));
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    m_shader.use();
    m_shader.setAttributeFloat("position", 2, 4 * sizeof(float), 0);
    m_shader.setAttributeFloat("texcoord", 2, 4 * sizeof(float), 2 * sizeof(float));
    auto model = glm::translate(glm::mat4(1.f), glm::vec3{0.5, 0.5, 0.5});
    model = glm::scale(model, glm::vec3{0.6, 0.6, 1.f});
    m_shader.setUniform("model", model);
    m_vao.unbind();
    using namespace fly;
    ASSERT_GL_ERRORS();
}
void Sprite::draw()
{
    glDisable(GL_DEPTH_TEST);
    m_shader.use();
    m_vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    m_vao.unbind();
    glEnable(GL_DEPTH_TEST);
}
void Sprite::setTexture(int texture)
{
    m_shader.setUniform("tex", texture);
}


}
