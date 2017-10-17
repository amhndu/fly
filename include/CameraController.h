#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H
#include <SFML/Window/Window.hpp>
#include "Camera.h"

namespace fly
{


class CameraController
{
public:
    CameraController(sf::Window& window, Camera& camera);
    void update(float dt);
    void passEvent(const sf::Event& event);
private:
    sf::Window&     m_window;
    Camera&         m_camera;
    sf::Vector2i    m_previousMouse;
    bool            m_mouseDown;
};


}


#endif
