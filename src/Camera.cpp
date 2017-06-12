#include "Camera.h"
#include "Log.h"
#include <glm/gtc/matrix_transform.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cmath>

namespace fly
{

Camera::Camera(ShaderProgram& shader, glm::vec3 position, glm::vec3 direction, glm::vec3 up,
               glm::mat4 defaultProj, sf::Window& window) :
                    m_shader(shader),
                    m_window(window),
                    m_prevMouse(sf::Mouse::getPosition(window)),
                    m_position(position),
                    m_direction(direction),
                    m_up(up),
                    m_viewChanged(true),
                    m_view(glm::lookAt(position, direction + position, up)),
                    m_projChanged(true),
                    m_projection(defaultProj)
{
    m_prevMouse = static_cast<sf::Vector2i>(m_window.getSize()) / 2;
    sf::Mouse::setPosition(m_prevMouse, window);
}


void Camera::updateView(float dt)
{
    auto newMouse = sf::Mouse::getPosition(m_window);
    if (newMouse != m_prevMouse)
    {
        auto diff = newMouse - m_prevMouse;
        if (diff.y)
        {
            diff.y = -diff.y;
            float theta = M_PI / 8.f * diff.y / m_window.getSize().y;
            auto prev_dir = m_direction, prev_up = m_up;
            m_direction = glm::normalize(std::cos(theta) * prev_dir + std::sin(theta) * prev_up);
            m_up        = glm::normalize(std::cos(theta) * prev_up  + std::sin(theta) * prev_dir);
            m_viewChanged = true;
        }
        if (diff.x)
        {
            float theta = M_PI / 8.f * diff.x / m_window.getSize().x;
            auto prev_dir = m_direction, prev_up = m_up;
            auto cross = glm::cross(m_direction, m_up);
            m_direction = glm::normalize(std::cos(theta) * prev_dir + std::sin(theta) * cross);
//             m_up        = glm::normalize(std::cos(theta) * prev_up  + std::sin(theta) * cross);
            m_viewChanged = true;
        }

        if (m_viewChanged)
        {
            if (std::abs(newMouse.x - (int)(m_window.getSize().x) / 2) >= m_window.getSize().x / 4
             || std::abs(newMouse.y - (int)(m_window.getSize().y) / 2) >= m_window.getSize().y / 4)
            {
                m_prevMouse = static_cast<sf::Vector2i>(m_window.getSize()) / 2;
                sf::Mouse::setPosition(m_prevMouse, m_window);
            }
            else
                m_prevMouse = newMouse;
        }
    }

    bool negate = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || (negate = sf::Keyboard::isKeyPressed(sf::Keyboard::Down)))
    {
        float velocity = 1.0f;
        if (negate) velocity *= -1.f;
        m_position += m_direction * velocity * dt;
        m_viewChanged = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || (negate = sf::Keyboard::isKeyPressed(sf::Keyboard::Right)))
    {
        float velocity = 1.0f;
        if (negate) velocity *= -1.f;
        m_position += glm::cross(m_up, m_direction) * velocity * dt;
        m_viewChanged = true;
    }

    if (m_viewChanged)
    {
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

}
