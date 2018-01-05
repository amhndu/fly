#ifndef TERRAINRENDERER_H
#define TERRAINRENDERER_H

#include <GL/glew.h>
#include <vector>
#include <map>
#include <cstdint>
#include "Shader.h"
#include "Vertex.h"
#include "VertexArrayObject.h"
#include "Renderer.h"

namespace fly
{

class TerrainRenderer : public Renderer
{
public:
    TerrainRenderer(int radius, int detail);
    ~TerrainRenderer();
    void reset(int radius, int detail);
    void updateChunk(int chunk_x, int chunk_y,
                     int coord_x, int coord_y,
                     const std::vector<float>& chunk_heights,
                     const std::vector<float>& colormap);
    void draw() override;
    void rawDraw() override;
    void setView(const glm::mat4& view)      override { m_shaderProgram.setUniform("view", view); }
    void setProjection(const glm::mat4& proj)override { m_shaderProgram.setUniform("proj", proj); }
    void setLightDirection(const glm::vec3& dir) { m_shaderProgram.setUniform("light_direction", dir);}
    void setLightSpace(const glm::mat4& lm)   { m_shaderProgram.setUniform("lightSpace", lm); }
private:
    ShaderProgram m_shaderProgram;

    VertexArrayObject m_vao;
    GLuint m_vertexBuffer;
    GLuint m_elementBuffer;

    int m_radius;
    int m_detail;
    int m_chunks;
    int m_verticesPerChunk;
    int m_elementsPerChunk;
};

}
#endif // TERRAINRENDERER_H
