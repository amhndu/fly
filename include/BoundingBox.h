#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace fly
{

struct AABB
{
    glm::vec3 position;
    glm::vec3 dimensions;
};

struct OBB
{
    OBB(const glm::vec3& dimensions, const glm::mat4& transformation);
    AABB getAABB() const;

    glm::vec3 points[8];
};

}
#endif //BOUNDINGBOX_H
