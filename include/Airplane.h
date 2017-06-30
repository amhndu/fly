#ifndef AIRPLANE_H
#define AIRPLANE_H
#include <glm/glm.hpp>
#include "Model.h"

namespace fly
{


class Airplane
{
public:
    Airplane();
    void roll(char direction) { m_aileron = direction; }
    void elevate(char direction) { m_elevator = direction; }
    void draw();
    void update(float dt);
    void setProjection(const glm::mat4& proj) { m_model.setProjection(proj); }
    void setView(const glm::mat4& view) { m_model.setView(view); }
    const glm::vec3& getPosition() const { return m_position; }
    const glm::vec3& getForwardDirection() const { return m_forward; }
    const glm::vec3& getUpDirection() const { return m_up; }
private:
    glm::vec3 m_position;
    glm::vec3 m_forward;
    glm::vec3 m_up;
    glm::vec3 m_left;
    glm::vec3 m_velocity;
    glm::mat4 m_translationMatrix;
    glm::mat4 m_rotationMatrix;
    char m_aileron;
    char m_elevator;
    Model m_model;
};

}

#endif // AIRPLANE_H
