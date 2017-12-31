#ifndef PARTICLEUPDATERS_H
#define PARTICLEUPDATERS_H
#include <functional>
#include "ParticleData.h"


namespace fly
{

using ParticleUpdater = std::function<void(float, ParticleData&)>;

namespace Updater
{


ParticleUpdater createConstAcceleration(const glm::vec3& acc);

ParticleUpdater createAxialAcceleration(const glm::vec3& center, const glm::vec3& direction, float intensity);

ParticleUpdater createLinearSize(float begin, float end);

void lifeUpdater(float dt, ParticleData& data);

void basicPhysics(float dt, ParticleData& data);

void fireColor(float dt, ParticleData& data);

}


}


#endif
