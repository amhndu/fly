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
//     m_prevMouse = static_cast<sf::Vector2i>(m_window.getSize()) / 2;
//     sf::Mouse::setPosition(m_prevMouse, window);
}


void Controller::takeInput(float dt)
{
    // some shorthands..
    using Key       = sf::Keyboard;
    auto  isPressed = sf::Keyboard::isKeyPressed;

    if (isPressed(Key::W) || isPressed(Key::Up))
        m_callbacks[ElevatorDown]();
    if (isPressed(Key::S) || isPressed(Key::Down))
        m_callbacks[ElevatorUp]();

    if (isPressed(Key::A) || isPressed(Key::Left))
        m_callbacks[RollLeft]();
    if (isPressed(Key::D) || isPressed(Key::Right))
        m_callbacks[RollRight]();

    if (isPressed(Key::Space))
        m_callbacks[ThrustUp]();
    if (isPressed(Key::LShift))
        m_callbacks[ThrustDown]();

//     auto newMouse = sf::Mouse::getPosition(m_window);
//     if (newMouse != m_prevMouse)
//     {
//         m_rotateCallback((newMouse.x - m_prevMouse.x) / static_cast<float>(m_window.getSize().x),
//                          (newMouse.y - m_prevMouse.y) / static_cast<float>(m_window.getSize().y));
//
//         if ( std::abs(newMouse.x - (int)(m_window.getSize().x) / 2) >= (int)m_window.getSize().x / 4
//           || std::abs(newMouse.y - (int)(m_window.getSize().y) / 2) >= (int)m_window.getSize().y / 4)
//         {
//             m_prevMouse = static_cast<sf::Vector2i>(m_window.getSize()) / 2;
//             sf::Mouse::setPosition(m_prevMouse, m_window);
//         }
//         else
//             m_prevMouse = newMouse;
//     }
}

}
