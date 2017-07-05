#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include <SFML/Window.hpp>
#include "Shader.h"
#include "Airplane.h"

namespace fly
{

class Camera
{
public:
    Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up, Airplane& airplane);
    bool viewChanged() const { return m_viewChanged; }
    void updateView(float dt);
    glm::mat4 getView();

private:
    glm::vec3 m_position;
    glm::vec3 m_direction;
    glm::vec3 m_up;
    glm::vec3 m_planeUp;

    bool      m_viewChanged;

    glm::mat4 m_view;

    Airplane& m_airplane;
};

}

#endif // CAMERA_H
