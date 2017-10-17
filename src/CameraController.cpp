#include "CameraController.h"
#include "Log.h"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>

namespace fly
{

CameraController::CameraController(sf::Window& window, Camera& camera) :
    m_window(window),
    m_camera(camera),
    m_previousMouse(sf::Mouse::getPosition(window)),
    m_mouseDown(false)
{
}

void CameraController::update(float dt)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        auto new_mouse = sf::Mouse::getPosition(m_window);
        if (  new_mouse.x < 0 || new_mouse.x > m_window.getSize().x
           || new_mouse.y < 0 || new_mouse.y > m_window.getSize().y)
            return;

        if (m_mouseDown)
        {
            auto diff = new_mouse - m_previousMouse;
            m_camera.rotate(diff.x * dt, diff.y * dt);
        }
        m_mouseDown = true;
        m_previousMouse = new_mouse;
    }
    else
        m_mouseDown = false;
}

void CameraController::passEvent(const sf::Event& event)
{
    if (event.type == sf::Event::MouseWheelScrolled)
    {
        m_camera.zoom(event.mouseWheelScroll.delta * -0.05f);
    }
}

}
