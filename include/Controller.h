#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <functional>
#include <SFML/Window.hpp>

namespace fly
{


class Controller
{
public:
    Controller(sf::Window& window);
    template <class Callable>
    void registerMove(Callable cb) { m_moveCallback = cb; }
    template <class Callable>
    void registerRotate(Callable cb) { m_rotateCallback = cb; }
    void takeInput(float dt);
private:
    std::function<void(float, float, float)> m_moveCallback;
    std::function<void(float, float)> m_rotateCallback;
    sf::Window& m_window;
    sf::Vector2i m_prevMouse;
};

}

#endif // CONTROLLER_H
