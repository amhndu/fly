#include "Camera.h"
#include "Log.h"
#include "Utility.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

namespace fly
{

Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up, Airplane& airplane) :
                    m_position(position),
                    m_direction(direction),
                    m_up(up),
                    m_viewChanged(true),
                    m_view(glm::lookAt(position, direction + position, up)),
                    m_airplane(airplane)
{
}

glm::mat4 Camera::getView()
{
    if (m_viewChanged)
    {
        m_view = glm::lookAt(m_position -
            glm::normalize(glm::vec3{m_direction.x, m_direction.y, 0.f}) * 0.2f
                         + glm::vec3{0.f, 0.f, 1.f} * 0.08f,  // eye
            m_position,  // center
            m_up);
        m_viewChanged = false;
    }
    return m_view;
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

void Camera::updateView(float dt)
{
    m_position    = m_airplane.getPosition();
    auto delta_direction = m_airplane.getForwardDirection() - m_direction;
    delta_direction.x  = sign(delta_direction.x) * std::min(std::abs(delta_direction.x), 0.008f);
    delta_direction.y  = sign(delta_direction.y) * std::min(std::abs(delta_direction.y), 0.008f);
    m_direction += delta_direction;
    m_planeUp     = m_airplane.getUpDirection();
    m_viewChanged = true;
}

}
