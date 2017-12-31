#include "Debug/Box.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Utility.h"

namespace fly
{

Box::Box()
{
    m_vao.bind();
    float vertices[] = {
                -0.5f, -0.5f, -0.5f,
                 0.5f, -0.5f, -0.5f,
                 0.5f,  0.5f, -0.5f,
                 0.5f,  0.5f, -0.5f,
                -0.5f,  0.5f, -0.5f,
                -0.5f, -0.5f, -0.5f,

                -0.5f, -0.5f,  0.5f,
                 0.5f, -0.5f,  0.5f,
                 0.5f,  0.5f,  0.5f,
                 0.5f,  0.5f,  0.5f,
                -0.5f,  0.5f,  0.5f,
                -0.5f, -0.5f,  0.5f,

                -0.5f,  0.5f,  0.5f,
                -0.5f,  0.5f, -0.5f,
                -0.5f, -0.5f, -0.5f,
                -0.5f, -0.5f, -0.5f,
                -0.5f, -0.5f,  0.5f,
                -0.5f,  0.5f,  0.5f,

                 0.5f,  0.5f,  0.5f,
                 0.5f,  0.5f, -0.5f,
                 0.5f, -0.5f, -0.5f,
                 0.5f, -0.5f, -0.5f,
                 0.5f, -0.5f,  0.5f,
                 0.5f,  0.5f,  0.5f,

                -0.5f, -0.5f, -0.5f,
                 0.5f, -0.5f, -0.5f,
                 0.5f, -0.5f,  0.5f,
                 0.5f, -0.5f,  0.5f,
                -0.5f, -0.5f,  0.5f,
                -0.5f, -0.5f, -0.5f,

                -0.5f,  0.5f, -0.5f,
                 0.5f,  0.5f, -0.5f,
                 0.5f,  0.5f,  0.5f,
                 0.5f,  0.5f,  0.5f,
                -0.5f,  0.5f,  0.5f,
                -0.5f,  0.5f, -0.5f,
    };


    const std::string vertex_shader = R"gl(
        #version 330 core
        layout (location = 0) in vec3 position;
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 proj;

        void main()
        {
            gl_Position = proj * view * model * vec4(position, 1.0);
        }
        )gl";
    const std::string fragment_shader = R"gl(
        #version 330 core
        out vec4 outColor;
        void main()
        {
            outColor = vec4(vec3(0.1), 0.2);
        }
        )gl";

    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    assert(m_shader.loadShaderString(vertex_shader,   fly::Shader::Vertex));
    assert(m_shader.loadShaderString(fragment_shader, fly::Shader::Fragment));
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    m_shader.use();
    m_shader.setAttributeFloat("position", 3, sizeof(float) * 3, 0);
    m_vao.unbind();
    ASSERT_GL_ERRORS();
}

void Box::draw()
{
    m_shader.use();
    m_vao.bind();
    glDepthMask(GL_FALSE);
//    glDisable(GL_CULL_FACE);
    glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
//     glEnable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);
    m_vao.unbind();
}


}
