#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H
#include <vector>
#include "ParticleRenderer.h"
#include "ParticleData.h"
#include "ParticleEmitters.h"
#include "ParticleUpdaters.h"

namespace fly
{


class ParticleSystem
{

public:
    ParticleSystem(int max_particles = 0) : m_data(max_particles), m_renderer(m_data) {}
    ParticleRenderer& getRenderer() { return m_renderer; }
    void setParticleSize(float size);
    void setMaxAge(float age);
    void update(float deltaTime);
    void reset(int max_particles);
    void addUpdater(ParticleUpdater updater) { m_updaters.push_back(updater); }
    void addEmitter(ParticleEmitter emitter) { m_emitters.push_back(emitter); }
private:
    ParticleData                    m_data;
    ParticleRenderer                m_renderer;
    std::vector<ParticleUpdater>    m_updaters;
    std::vector<ParticleEmitter>    m_emitters;
};

    /*
    Fire example:
    ps.setParticleSize(0.017f);
    ps.addEmitter(Emitter::createUniformCone(50.f, aircraft.getPosition(), {0.025f, 0.025f, 0.008f},
                                             {0.f, 0.f, 0.025f}, 60.f * M_PI / 180.f));
    ps.addUpdater(Updater::basicPhysics);
    ps.addUpdater(Updater::createLifeUpdater(5.f));
    ps.addUpdater(Updater::createAxialAcceleration(aircraft.getPosition(), aircraft.getUpDirection(), 1.05f));
    ps.addUpdater(Updater::createFireColor(5.f));
    */

}

#endif //PARTICLESYSTEM_H
