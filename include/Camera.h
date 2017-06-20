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
           glm::mat4 defaultProj);
    void updateView();
    void rotate(float x, float y);
    glm::vec2 move(float dx, float dy, float dz);
private:
    ShaderProgram& m_shader;

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
