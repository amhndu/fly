#include "Airplane.h"
#include "Log.h"
#include "Utility.h"
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <algorithm>

namespace fly
{


Airplane::Airplane() :
    m_position({0.0f, 0.0f, 1.2f}),
    m_forward({1.f, 0.f, 0.f}),
    m_up({0.f, 0.f, 1.f}),
    m_left({0.f, 1.f, 0.f}),
    m_speed(1.0f),
    m_velocity(glm::vec3(1.f) * m_speed),
    m_translationMatrix(glm::translate(glm::mat4(1.f), m_position)),
    m_rotationMatrix(1.f),
    m_aileron(0),
    m_rollVelocity(0),
    m_elevator(0),
    m_pitchVelocity(0),
    m_throttle(0),
    m_model("resources/airplane.obj")
{
    auto transform = m_translationMatrix;
    m_model.setTransform(transform);
}

void Airplane::update(float dt)
{
    if (m_throttle)
    {
        m_speed += 0.5 * m_throttle * dt;
        m_speed = std::min(std::max(0.f, m_speed), 3.f);
        m_throttle = 0;
    }


    float target_velocity, rate;
    bool positive_velocity;
    if (m_aileron)
    {
        target_velocity = PI / 3.f * m_aileron;
        positive_velocity = m_aileron > 0;
        rate = 0.025f;
    }
    else
    {
        target_velocity = sign(m_up.z) * -sign(m_left.z) * std::sqrt(std::abs(m_left.z)) * PI / 6.f;
        positive_velocity = sign(m_up.z) * -sign(m_left.z) > 0;
        rate = 0.014f;
    }

    using ClampFunction = const float& (*)(const float&, const float&);

    auto clamping_func = positive_velocity ? ClampFunction(std::min) : ClampFunction(std::max);
    m_rollVelocity = clamping_func(m_rollVelocity + target_velocity * rate, target_velocity);
    float dAngleX = m_rollVelocity * dt;
    if (std::abs(dAngleX) > 1e-5)
        m_rotationMatrix = glm::rotate(m_rotationMatrix, dAngleX, {1.f, 0.f, 0.f});


    if (m_elevator)
    {
        target_velocity = (PI / 4.f * (1.f - ((m_left.z)))) * m_elevator;
        positive_velocity = m_elevator > 0;
        rate = 0.04f;
    }
    else
    {
        target_velocity = sign(m_up.z) * sign(m_forward.z) * std::sqrt(std::abs(m_forward.z)) * PI / 4.f;
        positive_velocity = sign(m_up.z) * sign(m_forward.z) > 0;
        rate = 0.03f;
    }
    clamping_func = positive_velocity ? ClampFunction(std::min) : ClampFunction(std::max);
    m_pitchVelocity = clamping_func(m_pitchVelocity + target_velocity * rate, target_velocity);
    float dAngleY = m_pitchVelocity * dt;
    if (std::abs(dAngleY) > 1e-5)
        m_rotationMatrix = glm::rotate(m_rotationMatrix, dAngleY, {0.f, 1.f, 0.f});


    m_forward = glm::normalize(m_rotationMatrix[0]);
    m_left    = glm::normalize(m_rotationMatrix[1]);
    m_up      = glm::normalize(m_rotationMatrix[2]);


    auto thrust  =  m_forward * 15.0f * m_speed / 1.0f;
    auto drag    = -glm::normalize(m_velocity) * (15.0f / sq(1.0f)) * glm::dot(m_velocity, m_velocity);
    glm::vec3 gravity =  glm::vec3(0, 0, -1) * 2.f;
    glm::vec3 lift    =  {0.f, 0.f, (m_up * (2.f / 1.0f) * sq(glm::dot(m_forward, m_velocity))).z};

    float cosine = std::abs(m_left.z);
    if (cosine >= 0.1)
    {
        float radius = 3.8f / cosine;
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
    m_translationMatrix = glm::translate(glm::mat4{1.f}, m_position);

    auto transform = m_translationMatrix * m_rotationMatrix;
    m_model.setTransform(transform);

    m_aileron = m_elevator = 0;
}


}
