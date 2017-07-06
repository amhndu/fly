#include "Airplane.h"
#include "Log.h"
#include "Utility.h"
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

namespace fly
{


Airplane::Airplane() :
    m_position({0.4f, 0.0f, 1.9f}),
    m_forward({1.f, 0.f, 0.f}),
    m_up({0.f, 0.f, 1.f}),
    m_left({0.f, 1.f, 0.f}),
    m_speed(1.5f),
    m_velocity(glm::vec3(1.f) * m_speed),
    m_translationMatrix(glm::translate(glm::mat4(1.f), m_position)),
    m_rotationMatrix(1.f),
    m_aileron(0),
    m_elevator(0),
    m_throttle(0),
    m_stabilizeTimer(0),
    m_model("resources/airplane.obj")
{
    auto transform = m_translationMatrix;
    m_model.setTransform(transform);
}

void Airplane::draw()
{
    m_model.draw();
}

void Airplane::update(float dt)
{
    if (m_throttle)
    {
        m_speed += 0.5 * m_throttle * dt;
        m_speed = std::min(std::max(0.f, m_speed), 3.f);
        m_throttle = 0;
    }

    if (m_stabilizeTimer > 0)
        m_stabilizeTimer -= dt;
    float dAngleX = 0.f;
    if (m_aileron) // roll
        dAngleX     = M_PI / 3.f * m_aileron * dt;
    else if (m_stabilizeTimer < 0)
        dAngleX     = -sign(m_left.z) * std::sqrt(std::abs(m_left.z)) * M_PI / 8.f * dt;
    if (dAngleX != 0.f)
        m_rotationMatrix = glm::rotate(m_rotationMatrix, dAngleX, {1.f, 0.f, 0.f});

    float dAngleY = 0.f;
    if (m_elevator)
        dAngleY     = (M_PI / 3.f * (1.f - sq(sq(m_left.z)))) * m_elevator * dt;
    else if (m_stabilizeTimer < 0)
        dAngleY     = m_forward.z * M_PI / 6.f * dt;
    if (dAngleY != 0.f)
        m_rotationMatrix = glm::rotate(m_rotationMatrix, dAngleY, {0.f, 1.f, 0.f});

    if (m_elevator || m_aileron)
        m_stabilizeTimer = 0.3f;

    m_forward = glm::normalize(m_rotationMatrix[0]);
    m_left    = glm::normalize(m_rotationMatrix[1]);
    m_up      = glm::normalize(m_rotationMatrix[2]);

    auto thrust  =  m_forward * 25.0f * m_speed / 1.5f;
    auto drag    = -glm::normalize(m_velocity) * (25.0f / sq(1.5f)) * glm::dot(m_velocity, m_velocity);
    glm::vec3 gravity =  glm::vec3(0, 0, -1) * 15.f;
    glm::vec3 lift    =  {0.f, 0.f, (m_up * (15.f / sq(1.5f)) * sq(glm::dot(m_forward, m_velocity))).z};

    float sine = std::sqrt(1 - sq(glm::dot(m_up, glm::vec3{0.f, 0.f, 1.f})));
    if (sine >= 0.1)
    {
        float radius = 4.5f / sine;
        auto centripetal = glm::normalize(glm::vec3{m_up.x, m_up.y, 0.f}) * glm::dot(m_velocity, m_velocity) / radius;
        lift += centripetal;
        auto direction = sign(glm::cross(m_velocity, centripetal).z);
        if (direction !=  0.f)
        {
            // rotate the model and vectors
            glm::vec3 axis = glm::inverse(m_rotationMatrix) * glm::vec4(0.f, 0.f, direction, 0.f);
            m_rotationMatrix = glm::rotate(m_rotationMatrix, glm::length(m_velocity) / radius * dt, axis);
            m_forward = glm::normalize(m_rotationMatrix[0]);
            m_left    = glm::normalize(m_rotationMatrix[1]);
            m_up      = glm::normalize(m_rotationMatrix[2]);
        }
    }

    glm::vec3 acceleration = thrust + drag + gravity + lift;

    m_velocity += acceleration * dt;

    m_position += m_velocity * dt;
    m_translationMatrix = glm::translate({}, m_position);

    auto transform = m_translationMatrix * m_rotationMatrix;
    m_model.setTransform(transform);

    m_aileron = m_elevator = 0;
}


}
