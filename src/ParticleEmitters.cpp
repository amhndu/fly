#include "ParticleEmitters.h"
#include "Log.h"
#include "Utility.h"
#include <glm/geometric.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <cmath>

namespace fly
{

namespace
{
// TODO use real_distribution from <random>
float get_rand()
{
    return rand() / (float) RAND_MAX;
}

class UniformConeImpl
{
public:
    UniformConeImpl(float rate, const glm::vec3& startingPosition, const glm::vec3& posVariance,
                    const glm::vec3& startingVelocity, float lifeMin, float lifeMax, float sizeMin, float sizeMax,
                    float angle, int particlesToEmit = -1)
        : m_rate(rate)
        , m_timer(0)
        , m_startingPosition(startingPosition)
        , m_posVariance(posVariance)
        , m_startingVelocity(startingVelocity)
        , m_velPerpendicular(glm::normalize(glm::vec3{0.f, -m_startingVelocity.z, m_startingVelocity.y}))
        , m_lifeMin(lifeMin)
        , m_lifeMax(lifeMax)
        , m_sizeMin(sizeMin)
        , m_sizeMax(sizeMax)
        , m_angle(angle)
        , m_infiniteParticles(particlesToEmit < 0)
        , m_particlesToEmit(particlesToEmit)
    {
        LOG(Debug) << "Infinite particles: " << std::boolalpha << m_infiniteParticles << std::endl;
        LOG(Debug) << "Particles to emit: " << m_particlesToEmit << std::endl;
    }

    void operator() (float dt, ParticleData& data)
    {
        if (!m_infiniteParticles && m_particlesToEmit <= 0)
            return;

        m_timer += dt;
        int new_particles = m_timer * m_rate;
        if (!m_infiniteParticles)
            new_particles = std::min<float>(new_particles, m_particlesToEmit);
        for (int i = 0; i < new_particles; ++i, m_infiniteParticles || --m_particlesToEmit)
        {
            int pi = data.create();
            if (pi != -1)
            {
                data.positionArray[pi] = m_startingPosition + m_posVariance *
                            glm::vec3{2 * get_rand() - 1, 2 * get_rand() - 1, 2 * get_rand() - 1};
                // to get a uniform (spherical) distribution ... probably overkill
                auto a = 2 * std::asin(std::sqrt(sq(std::sin(get_rand() * m_angle / 2.f))));
                auto v = glm::rotate(m_startingVelocity, a, m_velPerpendicular);
                v = glm::rotate(v, (float) PI * 2.f * get_rand(), m_startingVelocity);
                data.velocityArray[pi] = v;
                data.maxAgeArray[pi]   = lerp(get_rand(), m_lifeMin, m_lifeMax);
                data.sizeArray[pi]     = lerp(get_rand(), m_sizeMin, m_sizeMax);
                data.ageArray[pi]      = 0;
            }
            else
            {
                LOG(Debug) << "Particle pool full" << std::endl;
                break;
            }
        }
        m_timer = m_timer - new_particles / m_rate;
    }
    bool isEmitting() { return m_infiniteParticles || m_particlesToEmit > 0; }
    void moveStartPosition(const glm::vec3& delta) { m_startingPosition += delta; }
    void setStartVelocity(const glm::vec3& vel)
    {
        m_startingVelocity  = vel;
        m_velPerpendicular = glm::normalize(glm::vec3{0.f, -m_startingVelocity.z, m_startingVelocity.y});
    }
private:
    float m_rate;
    float m_timer;
    glm::vec3 m_startingPosition;
    glm::vec3 m_posVariance;
    glm::vec3 m_startingVelocity;
    glm::vec3 m_velPerpendicular;
    float     m_lifeMin;
    float     m_lifeMax;
    float     m_sizeMin;
    float     m_sizeMax;
    float     m_angle;
    bool      m_infiniteParticles;
    int       m_particlesToEmit;
};


class ExplosionEmitter
{
public:
    ExplosionEmitter(const glm::vec3& center, const glm::vec3& up, const glm::vec3& gravity,
                     float emitter_speed)
        : m_center(center)
        , m_up(glm::normalize(up))
        , m_gravity(gravity)
    {
        auto velPerpendicular = glm::normalize(glm::vec3{0.f, -m_up.z, m_up.y});
        const int sub_emitters = 10;
        m_subEmitters.reserve(sub_emitters);
        for (int i = 0; i < sub_emitters; ++i)
        {
            auto em_vel = glm::rotate(emitter_speed * m_up * lerp(get_rand(), 0.6f, 1.f),
                                      (float) lerp(get_rand(), PI / 12.f, PI / 3.f), velPerpendicular);
            em_vel = glm::rotate(em_vel, (float) PI * 2.f * get_rand(), m_up);
            m_subEmitters.push_back({em_vel,
                                UniformConeImpl(220, center, {}, {}, 2.f, 5.f,
                                                0.15f, 0.50f, PI / 2.f, 400)});
        }
    }
    void operator() (float dt, ParticleData& data)
    {
        for (auto& sub_em : m_subEmitters)
        {
            sub_em.velocity += m_gravity * dt;
            sub_em.impl.moveStartPosition(sub_em.velocity * dt);
            sub_em.impl.setStartVelocity(glm::normalize(sub_em.velocity) * 0.008f);
            sub_em.impl(dt, data);
        }
    }
private:
    struct SubEmitterWrapper
    {
        glm::vec3 velocity;
        UniformConeImpl impl;
    };
    std::vector<SubEmitterWrapper> m_subEmitters;
    glm::vec3 m_center;
    glm::vec3 m_up;
    glm::vec3 m_gravity;
};

}

namespace Emitter
{


ParticleEmitter createUniformCone(float rate, const glm::vec3& startingPosition, const glm::vec3& posVariance,
                              const glm::vec3& startingVelocity, float minLife, float maxLife,
                              float sizeMin, float sizeMax, float angle, int particlesToEmit)
{
    return UniformConeImpl(rate, startingPosition, posVariance, startingVelocity,
                           minLife, maxLife, sizeMin, sizeMax, angle, particlesToEmit);
}

ParticleEmitter createExplosion(const glm::vec3& center, const glm::vec3& spread_up,
                                const glm::vec3& gravity, float emitter_speed)
{
    return ExplosionEmitter(center, spread_up, gravity, emitter_speed);
}

}

}
