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
                    m_up(glm::normalize(up)),
                    m_timer(0),
                    m_distance(0.2f),
                    m_stationary(true),
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
            glm::normalize(glm::vec3{m_direction.x, m_direction.y, m_direction.z}) * m_distance
                         + glm::vec3{0.f, 0.f, 1.f - m_direction.z} * (m_distance * 0.3f),  // eye
            m_position,  // center
            m_up);
        m_viewChanged = false;
    }
    return m_view;
}

void Camera::rotate(float x, float y)
{
    const float multiplier = PI / 6.f;
    if (y)
    {
        y = -y;
        float theta = multiplier * y;
        auto prev_dir = glm::normalize(m_direction), prev_up = glm::normalize(m_up);
        m_direction = glm::normalize(std::cos(theta) * prev_dir + std::sin(theta) * prev_up);
    }
    if (x)
    {
        float theta = multiplier * x;
        auto prev_dir = glm::normalize(m_direction);
        auto cross  = glm::normalize(glm::cross(m_direction, m_up));
        m_direction = glm::normalize(std::cos(theta) * prev_dir + std::sin(theta) * cross);
    }
    m_timer = 0.3f;
    m_viewChanged = true;
}

void Camera::zoom(float delta)
{
    m_distance += delta;
    m_viewChanged = true;
}

void Camera::updateView(float dt)
{
    m_position    = m_airplane.getPosition();

    auto delta_direction = m_airplane.getForwardDirection() - m_direction;
    auto len = glm::length(delta_direction);

    float len_delta = 0.2f * dt;
    if (m_timer > 0.f)
        m_timer -= dt;
    else if (not m_stationary)
    {
        if (len < len_delta)
        {
            m_direction += delta_direction;
        }
        else
            m_direction += glm::normalize(delta_direction) * len_delta;

        if (len < 1e-4f)
            m_stationary = true;
    }
    else if (len >= 1e-4f && m_stationary)
    {
        m_timer = 0.5f;
        m_stationary = false;
    }
    m_viewChanged = true;
}

}
