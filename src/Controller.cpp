#include "Controller.h"
#include "Log.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

namespace fly
{

Controller::Controller(sf::Window& window)  :
    m_window(window),
    m_prevMouse(sf::Mouse::getPosition(window))
{
    m_prevMouse = static_cast<sf::Vector2i>(m_window.getSize()) / 2;
    sf::Mouse::setPosition(m_prevMouse, window);
}


void Controller::takeInput(float dt)
{
    // Aliases to type less..
    using Key       = sf::Keyboard;
    auto  isPressed = sf::Keyboard::isKeyPressed;

    const float velocity = 0.5f;
    float dx = 0, dy = 0, dz = 0;
    bool moved = false;
    if (isPressed(Key::W) || isPressed(Key::Up))
        dx += (moved = true, +1 * dt);
    if (isPressed(Key::S) || isPressed(Key::Down))
        dx += (moved = true, -1 * dt);

    if (isPressed(Key::A) || isPressed(Key::Left))
        dy += (moved = true, -1 * dt);
    if (isPressed(Key::D) || isPressed(Key::Right))
        dy += (moved = true, +1 * dt);

    if (isPressed(Key::Space))
        dz += (moved = true, +1 * dt);
    if (isPressed(Key::LShift))
        dz += (moved = true, -1 * dt);

    if (moved)
        m_moveCallback(dx * velocity, dy * velocity, dz * velocity);

    auto newMouse = sf::Mouse::getPosition(m_window);
    if (newMouse != m_prevMouse)
    {
        m_rotateCallback((newMouse.x - m_prevMouse.x) / static_cast<float>(m_window.getSize().x),
                         (newMouse.y - m_prevMouse.y) / static_cast<float>(m_window.getSize().y));

        if ( std::abs(newMouse.x - (int)(m_window.getSize().x) / 2) >= (int)m_window.getSize().x / 4
          || std::abs(newMouse.y - (int)(m_window.getSize().y) / 2) >= (int)m_window.getSize().y / 4)
        {
            m_prevMouse = static_cast<sf::Vector2i>(m_window.getSize()) / 2;
            sf::Mouse::setPosition(m_prevMouse, m_window);
        }
        else
            m_prevMouse = newMouse;
    }
}

}
