#ifndef SHADOWMAP_H
#define SHADOWMAP_H
#include "Shader.h"
#include "Airplane.h"
#include "Terrain.h"
#include <vector>
#include <glm/mat4x4.hpp>

namespace fly
{

class ShadowMap
{
public:
    const uint Width  = 1024;
    const uint Height = 1024;
    ShadowMap(Airplane& plane);
    ~ShadowMap();
    // Updates the shadow map and returns the light space transformation matrix
    const glm::mat4 update();
private:
    Airplane&     m_airplane;
    glm::mat4     m_projection;
    GLuint        m_frameBuffer;
    GLuint        m_depthMap;
    ShaderProgram m_shaderProgram;
};

}

#endif //SHADOWMAP_H
