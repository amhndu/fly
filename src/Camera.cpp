#include "Camera.h"
#include "Log.h"
#include "Utility.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

namespace fly
{

Camera::Camera(ShaderProgram& shader, glm::vec3 position, glm::vec3 direction, glm::vec3 up,
               glm::mat4 defaultProj) :
                    m_shader(shader),
                    m_position(position),
                    m_direction(direction),
                    m_up(up),
                    m_viewChanged(true),
                    m_view(glm::lookAt(position, direction + position, up)),
                    m_projChanged(true),
                    m_projection(defaultProj)
{
}

void Camera::updateView()
{
    if (m_viewChanged)
    {
//          LOG(Debug) << "Direction: " << m_direction << std::endl;
//          LOG(Debug) << "Position: " << m_position<< std::endl;
        m_view = glm::lookAt(m_position,
                             m_direction + m_position,
                             m_up);
        m_shader.setUniform("view", m_view);
        m_viewChanged = false;
    }
    if (m_projChanged)
    {
        m_shader.setUniform("proj", m_projection);
        m_projChanged = false;
    }
}

glm::vec2 Camera::move(float dx, float dy, float dz)
{
    glm::vec3 delta (0.f, 0.f, 0.f);
//     delta += glm::normalize(glm::vec3{m_direction.x, m_direction.y, 0.f}) * dx;
    delta += glm::normalize(m_direction) * dx;
    delta += glm::normalize(glm::cross(m_up, m_direction)) * (-dy);
    delta += m_up * dz;
    m_position += delta;
    m_viewChanged = true;
    return {delta.x, delta.y};
}

void Camera::rotate(float x, float y)
{
    const float multiplier = M_PI / 6.f;
    if (y)
    {
        y = -y;
        float theta = multiplier * y;
        auto prev_dir = m_direction, prev_up = m_up;
        m_direction = glm::normalize(std::cos(theta) * prev_dir + std::sin(theta) * prev_up);
    }
    if (x)
    {
        float theta = multiplier * x;
        auto prev_dir = m_direction;
        auto cross  = glm::normalize(glm::cross(m_direction, m_up));
        m_direction = glm::normalize(std::cos(theta) * prev_dir + std::sin(theta) * cross);
    }
    m_viewChanged = true;
}

}
