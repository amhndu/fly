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
        m_view = glm::lookAt(m_position - m_direction * 0.4f + m_up * 0.15f,  // eye
                             m_position,  // center
                             m_up);
        m_viewChanged = false;
    }
    return m_view;
}


void Camera::updateView(float dt)
{
    m_position    = m_airplane.getPosition();
    auto delta_direction = m_airplane.getForwardDirection() - m_direction;
    delta_direction.z = sign(delta_direction.z) * std::max(std::min(std::abs(delta_direction.z), 0.005f), 0.0009f);
    m_direction += delta_direction;
    m_planeUp     = m_airplane.getUpDirection();
    m_viewChanged = true;
}

}
