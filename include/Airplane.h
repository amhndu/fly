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
    Model&  getRenderer() { return m_model; }
    void    roll   (char direction) { m_aileron = direction; }
    void    elevate(char direction) { m_elevator = direction; }
    void    throttle(char t)  {  m_throttle = t; }
    void    update(float dt);
    const   glm::vec3& getPosition() const { return m_position; }
    const   glm::vec3& getForwardDirection() const { return m_forward; }
    const   glm::vec3& getUpDirection() const { return m_up; }
    const   glm::mat4  getModel(){ return m_translationMatrix * m_rotationMatrix; }
    const   AABB&      getLocalBounds() { return m_model.getLocalBounds(); }
    void    crash() { m_model.darken(); }
private:
    glm::vec3 m_position;
    glm::vec3 m_forward;
    glm::vec3 m_up;
    glm::vec3 m_left;
    float     m_speed;
    glm::vec3 m_velocity;
    glm::mat4 m_translationMatrix;
    glm::mat4 m_rotationMatrix;
    char      m_aileron;
    float     m_rollVelocity;
    char      m_elevator;
    float     m_pitchVelocity;
    char      m_throttle;
    Model     m_model;
};

}

#endif // AIRPLANE_H
