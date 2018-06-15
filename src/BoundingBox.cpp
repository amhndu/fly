#include <glm/vec4.hpp>
#include "BoundingBox.h"
#include "Log.h"
#include "Utility.h"
#include <algorithm>
#include <numeric>

namespace fly
{


OBB::OBB(const glm::vec3& dimensions, const glm::mat4& transformation)
{
    glm::vec4 half_extent {dimensions / 2.0f, 1.f};
    for (auto i = 0u; i < 8; ++i)
    {
        glm::vec4 local_point {((i & 4) >> 1) - 1.f,
                               (i & 2) - 1.f,
                               ((i & 1) << 1) - 1.f,
                               1.f};
        local_point = local_point * half_extent;
        points[i] = transformation * local_point;
    }
}

AABB OBB::getAABB() const
{
    auto min = std::accumulate(std::begin(points), std::end(points), points[0],
                    [](const glm::vec3& a, const glm::vec3& b){ return component_wise_apply(a, b, std::min); });
    auto max = std::accumulate(std::begin(points), std::end(points), points[0],
                    [](const glm::vec3& a, const glm::vec3& b){ return component_wise_apply(a, b, std::max); });

    return {(min + max) / 2.f, max - min};
}



}
