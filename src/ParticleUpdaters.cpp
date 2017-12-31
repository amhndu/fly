#include "ParticleUpdaters.h"
#include "Log.h"
#include "Utility.h"
#include <glm/geometric.hpp>

namespace fly
{


namespace
{

void constAccImpl(float dt, ParticleData& data, const glm::vec3& acc)
{
    for (glm::vec3& velocity : data.velocityArray)
    {
        velocity += acc * dt;
    }
}

class AxialAccelImpl
{
public:
    AxialAccelImpl(const glm::vec3& center, const glm::vec3& direction, float intensity)
        : m_center(center)
        , m_axis(glm::normalize(direction))
        , m_intensity(intensity)
    {}
    void operator() (float dt, ParticleData& data)
    {
        for (int i = 0; i < data.particlesAlive; ++i)
        {
            glm::vec3 radius = m_center - data.positionArray[i];
            radius = radius - glm::dot(radius, m_axis) * m_axis;
            data.positionArray[i] += radius * m_intensity * dt;
        }
    }
private:
    glm::vec3 m_center;
    glm::vec3 m_axis;
    float     m_intensity;
};

void variableSizeImpl(float dt, ParticleData& data, float begin, float end)
{
    for (int i = 0; i < data.particlesAlive; ++i)
    {
        data.sizeArray[i] = lerp(data.ageArray[i] / data.maxAgeArray[i], begin, end);
    }
}

}


namespace Updater
{

ParticleUpdater createConstAcceleration(const glm::vec3& acc)
{
    return [=](float dt, ParticleData& data){ constAccImpl(dt, data, acc); };
}

ParticleUpdater createAxialAcceleration(const glm::vec3& center, const glm::vec3& direction, float intensity)
{
    return AxialAccelImpl(center, direction, intensity);
}

ParticleUpdater createLinearSize(float begin, float end)
{
    return [=](float dt, ParticleData& data){ variableSizeImpl(dt, data, begin, end); };
}

void basicPhysics(float dt, ParticleData& data)
{
    for (int i = 0; i < data.particlesAlive; ++i)
    {
        data.positionArray[i] += data.velocityArray[i] * dt;
    }
}

void fireColor(float dt, ParticleData& data)
{
    for (int i = 0; i < data.particlesAlive; ++i)
    {
        static auto startcolor = glm::vec4{194.f/255.f, 64.4f/255.f, 31.f/255.f, 1.f};
//         static auto startcolor = glm::vec4{251.f/255.f, 97.f/255.f, 38.f/255.f, 1.f};
        static auto midcolor   = glm::vec4{0.4f, 0.4f, 0.4f, 0.3f};
        static auto endcolor   = glm::vec4{0.f, 0.f, 0.f, 0.f};
        float t = 1.f * data.ageArray[i] / data.maxAgeArray[i];
//         if (t < 0.5f)
//             data.colorArray[i] = startcolor * (1 - 2.f * t) + midcolor * 2.f * t;
//         else
//             data.colorArray[i] = midcolor * 2.f * t + endcolor * (2.f * t - 1.f);
        data.colorArray[i] = startcolor * (1.f - t) + endcolor * t;
    }
}

void lifeUpdater(float dt, ParticleData& data)
{
    for (int i = 0; i < data.particlesAlive; )
    {
        if ((data.ageArray[i] += dt) > data.maxAgeArray[i])
            data.kill(i);
        else    ++i;
    }
}


}

}
