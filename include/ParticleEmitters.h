#ifndef PARTICLEEMITTERS_H
#define PARTICLEEMITTERS_H
#include "ParticleData.h"
#include <functional>

namespace fly
{

using ParticleEmitter = std::function<void(float, ParticleData&)>;

namespace Emitter
{

ParticleEmitter createUniformCone(float rate, const glm::vec3& startingPosition, const glm::vec3& posVariance,
                              const glm::vec3& startingVelocity, float minLife, float maxLife,
                              float sizeMin, float sizeMax, float coneAngle, int particlesToEmit = -1);

ParticleEmitter createExplosion(const glm::vec3& center, const glm::vec3& spread_up,
                                const glm::vec3& gravity, float emitter_speed);

}


}
#endif
