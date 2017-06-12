#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include <SFML/Window.hpp>
#include "Shader.h"

namespace fly
{

class Camera
{
public:
    Camera(ShaderProgram& shader, glm::vec3 position, glm::vec3 direction, glm::vec3 up,
           glm::mat4 defaultProj, sf::Window& window);
    void updateView(float dt);
private:
    ShaderProgram& m_shader;
    sf::Window& m_window;

    sf::Vector2i m_prevMouse;
    glm::vec3 m_position;
    glm::vec3 m_direction;
    glm::vec3 m_up;
    bool m_viewChanged;
    glm::mat4 m_view;

    float m_fov;
    bool m_projChanged;
    glm::mat4 m_projection;
};

}

#endif // CAMERA_H
