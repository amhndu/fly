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
    Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up);
    bool viewChanged() const { return m_viewChanged; }
    glm::mat4 getView();
    void rotate(float x, float y);
    glm::vec2 move(const glm::vec3& dr);
    void setPosition(const glm::vec3& pos) { m_position = pos; m_viewChanged = true; }
    void setDirection(const glm::vec3& dir) { m_direction = dir; m_viewChanged = true; }
private:
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
