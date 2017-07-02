#include "Airplane.h"
#include "Log.h"
#include "Utility.h"
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

namespace fly
{

const float the_velocity = 1.5f;

Airplane::Airplane() :
    m_position({0.4f, 0.0f, 1.9f}),
    m_forward({1.f, 0.f, 0.f}),
    m_up({0.f, 0.f, 1.f}),
    m_left({0.f, 1.f, 0.f}),
    m_velocity(glm::vec3(1.f) * the_velocity),
    m_translationMatrix(glm::translate(glm::mat4(1.f), m_position)),
    m_rotationMatrix(1.f),
    m_aileron(0),
    m_elevator(0),
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
    if (m_aileron) // roll
    {
        float dAngle = M_PI / 2.f * m_aileron * dt;
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
    auto drag    = -glm::normalize(m_velocity) * (35.0f / sq(the_velocity)) * glm::dot(m_velocity, m_velocity);
    glm::vec3 gravity =  glm::vec3(0, 0, -1) * 25.f;
    glm::vec3 lift    =  {0.f, 0.f, (m_up * (25.f / sq(the_velocity)) * sq(glm::dot(m_forward, m_velocity))).z};


    float sine = std::sqrt(1 - sq(glm::dot(m_up, glm::vec3{0.f, 0.f, 1.f})));
    if (sine != 0)
    {
        float radius = 4.5f / sine;
        auto centripetal = glm::normalize(glm::vec3{m_up.x, m_up.y, 0.f}) * glm::dot(m_velocity, m_velocity) / radius;
        lift += centripetal;
        // rotate the model and vectors
        glm::vec3 axis = glm::inverse(m_rotationMatrix) * glm::vec4(0.f, 0.f, 1.f, 0.f);
        m_rotationMatrix = glm::rotate(m_rotationMatrix,
                                       sign(glm::dot(centripetal, m_left)) * glm::length(m_velocity) / radius * dt,
                                       axis);
        m_forward = glm::normalize(m_rotationMatrix[0]);
        m_left    = glm::normalize(m_rotationMatrix[1]);
        m_up      = glm::normalize(m_rotationMatrix[2]);
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
