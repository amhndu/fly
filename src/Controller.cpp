#include "Controller.h"
#include "Log.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

namespace fly
{

Controller::Controller(sf::Window& window)  :
    m_window(window)
{
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

}

}
