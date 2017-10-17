#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <functional>
#include <SFML/Window.hpp>

namespace fly
{


class Controller
{
public:
    enum Event
    {
        RollLeft,
        RollRight,
        ElevatorUp,         // Technically, elevator is down but we refer to the motion
        ElevatorDown,
        ThrustUp,
        ThrustDown,
        TotalEvents
    };
    Controller(sf::Window& window);
    template <class Callable>
    void setCallback(Event event, Callable cb) { if (event < TotalEvents) m_callbacks[event] = cb; }
    template <class Callable>
    void registerRotate(Callable cb) { m_rotateCallback = cb; }
    void takeInput(float dt);
private:
    std::function<void(float, float)> m_rotateCallback;
    std::function<void(void)> m_callbacks[TotalEvents];
    sf::Window& m_window;
};

}

#endif // CONTROLLER_H
