#ifndef SHADOWMAP_H
#define SHADOWMAP_H
#include "Shader.h"
#include "Airplane.h"
#include "Terrain.h"
#include "FrameBuffer.h"
#include <vector>
#include <glm/mat4x4.hpp>
#include <cstdint>

namespace fly
{

class ShadowMap
{
public:
    const uint Width  = 1024;
    const uint Height = 1024;
    ShadowMap(Airplane& plane);
    // Updates the shadow map and returns the light space transformation matrix
    const glm::mat4 update();
    void setLightDirection(const glm::vec3& dir) { m_lightDirection = dir; }
private:
    Airplane&     m_airplane;
    glm::mat4     m_projection;
    glm::vec3     m_lightDirection;
    FrameBuffer   m_frameBuffer;
//     GLuint        m_frameBuffer;
//     GLuint        m_depthMap;
    ShaderProgram m_shaderProgram;
};

}

#endif //SHADOWMAP_H
