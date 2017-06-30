#include "Camera.h"
#include "Log.h"
#include "Utility.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

namespace fly
{

Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up) :
                    m_position(position),
                    m_direction(direction),
                    m_up(up),
                    m_viewChanged(true),
                    m_view(glm::lookAt(position, direction + position, up))
{
}

glm::mat4 Camera::getView()
{
    if (m_viewChanged)
    {
        m_view = glm::lookAt(m_position - m_direction * 0.7f + glm::vec3(0, 0, 0.2f),  // eye
                             m_position,  // center
                             m_up);
        m_viewChanged = false;
    }
    return m_view;
}


glm::vec2 Camera::move(const glm::vec3& dr)
{
    glm::vec3 delta (0.f, 0.f, 0.f);
//     delta += glm::normalize(glm::vec3{m_direction.x, m_direction.y, 0.f}) * dx;
    delta += glm::normalize(m_direction) * dr.x;
    delta += glm::normalize(glm::cross(m_up, m_direction)) * (-dr.y);
    delta += m_up * dr.z;
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
