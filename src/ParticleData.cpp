#include "ParticleData.h"
#include "Log.h"
#include <utility>


namespace fly
{

ParticleData::ParticleData() : particlesAlive(0), m_maxParticles(0)
{
}

ParticleData::ParticleData(int particles)
{
    reset(particles);
}

void ParticleData::reset(int particles)
{
    particlesAlive = 0;
    m_maxParticles = particles;
    positionArray.resize(particles);
    velocityArray.resize(particles);
    colorArray.resize(particles, {1.f, 1.f, 1.f, 1.f});
    sizeArray.resize(particles);
    ageArray.resize(particles, 0.f);
    maxAgeArray.resize(particles);
}

int ParticleData::create()
{
    if (particlesAlive + 1 < m_maxParticles)
        return particlesAlive++;
    return -1;
}

void ParticleData::kill(std::size_t index)
{
    assert(particlesAlive > 0 && index < particlesAlive);
    --particlesAlive;
    // reset these attributes
    positionArray[index] = std::exchange(positionArray[particlesAlive], {});
    velocityArray[index] = std::exchange(velocityArray[particlesAlive], {});
    colorArray[index]    = std::exchange(colorArray[particlesAlive], {1.f, 1.f, 1.f, 1.f});
    ageArray[index]      = std::exchange(ageArray[particlesAlive], 0.f);
    // keep the size and maxAge
    std::swap(maxAgeArray[particlesAlive], maxAgeArray[index]);
    std::swap(sizeArray[particlesAlive], sizeArray[index]);
}


}
