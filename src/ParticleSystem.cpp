#include "ParticleSystem.h"
#include "TextureManager.h"
#include "Log.h"
#include "Utility.h"
#include <algorithm>

namespace fly
{

void ParticleSystem::setParticleSize(float size)
{
    std::fill(m_data.sizeArray.begin(), m_data.sizeArray.end(), size);
}

void ParticleSystem::setMaxAge(float age)
{
    std::fill(m_data.maxAgeArray.begin(), m_data.maxAgeArray.end(), age);
}

void ParticleSystem::reset(int max_particles)
{
    m_data.reset(max_particles);
    m_emitters.clear();
    m_updaters.clear();
}

void ParticleSystem::update(float deltaTime)
{
    for (auto&& emitter : m_emitters)
        emitter(deltaTime, m_data);

    for (auto&& updater : m_updaters)
        updater(deltaTime, m_data);
}


}
