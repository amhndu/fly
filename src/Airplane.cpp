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
    m_translationMatrix(glm::translate(glm::mat4(1.f), m_position)),
    m_rotationMatrix(1.f),
    m_aileron(0),
    m_elevator(0),
    m_model("airplane.obj")
{
    auto transform = m_translationMatrix;
    m_model.setTransform(transform);
    m_velocity = glm::normalize(m_forward) * 2.5f;
}

void Airplane::draw()
{
    m_model.draw();
}

void Airplane::update(float dt)
{
    if (m_aileron) // roll
    {
        float dAngle = M_PI / 3.f * m_aileron * dt;
        m_rotationMatrix = glm::rotate(m_rotationMatrix, dAngle, {1.f, 0.f, 0.f});
        m_left = glm::normalize(m_rotationMatrix[1]);
        m_up   = glm::normalize(m_rotationMatrix[2]);
    }

    if (m_elevator)
    {
        float dAngle = M_PI / 6.f * m_elevator * dt;
        m_rotationMatrix = glm::rotate(m_rotationMatrix, dAngle, {0.f, 1.f, 0.f});
        m_forward = glm::normalize(m_rotationMatrix[0]);
        m_up      = glm::normalize(m_rotationMatrix[2]);
    }

    // Utter simplification of physics.
    auto thrust  =  m_forward * 35.0f;
    auto drag    = -glm::normalize(m_velocity) * 5.6f * glm::dot(m_velocity, m_velocity);
    auto gravity =  glm::vec3(0, 0, -1) * 25.f;
    auto lift    =  m_up * 4.f * sq(glm::dot(m_forward, m_velocity));
//     LOG(Debug) << glm::dot(m_forward, m_velocity) << " " << lift.z << "   " << (gravity + lift).z << std::endl;


    m_velocity += (thrust + drag + gravity + lift) * dt;

    m_position += m_velocity * dt;
    m_translationMatrix = glm::translate({}, m_position);

    auto transform = m_translationMatrix * m_rotationMatrix;
    m_model.setTransform(transform);

    m_aileron = m_elevator = 0;
}


}
