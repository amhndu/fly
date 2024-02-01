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

    if (sf::Keyboard::isKeyPressed(Key::W) || sf::Keyboard::isKeyPressed(Key::Up))
        m_callbacks[ElevatorDown]();
    if (sf::Keyboard::isKeyPressed(Key::S) || sf::Keyboard::isKeyPressed(Key::Down))
        m_callbacks[ElevatorUp]();

    if (sf::Keyboard::isKeyPressed(Key::A) || sf::Keyboard::isKeyPressed(Key::Left))
        m_callbacks[RollLeft]();
    if (sf::Keyboard::isKeyPressed(Key::D) || sf::Keyboard::isKeyPressed(Key::Right))
        m_callbacks[RollRight]();

    if (sf::Keyboard::isKeyPressed(Key::Space))
        m_callbacks[ThrustUp]();
    if (sf::Keyboard::isKeyPressed(Key::LShift))
        m_callbacks[ThrustDown]();

}

}
