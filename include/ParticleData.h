#ifndef PARTICLEDATA_H
#define PARTICLEDATA_H
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace fly
{


struct ParticleData
{
    ParticleData();
    ParticleData(int particles);

    int         create();
    void        reset(int particles);
    void        kill(std::size_t index);

    int                    particlesAlive;
    std::vector<glm::vec3> positionArray;
    std::vector<glm::vec3> velocityArray;
    std::vector<glm::vec4> colorArray;
    std::vector<float>     sizeArray;
    std::vector<float>     ageArray;
    std::vector<float>     maxAgeArray;
private:
    int           m_maxParticles;
};


}

#endif // PARTICLEDATA_H
