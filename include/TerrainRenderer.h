#ifndef TERRAINRENDERER_H
#define TERRAINRENDERER_H

#include <GL/glew.h>
#include <vector>
#include <map>
#include <cstdint>
#include "Shader.h"
#include "Vertex.h"
#include "VertexArrayObject.h"

namespace fly
{

class TerrainRenderer
{
public:
    TerrainRenderer(int radius, int detail);
    ~TerrainRenderer();
    void reset(int radius, int detail);
    void updateChunk(int chunk_x, int chunk_y,
                     int coord_x, int coord_y,
                     float texture_size_x, float texture_size_y,
                     const std::vector<float>& chunk_heights,
                     const std::vector<float>& colormap);
    void draw();
    void setView(const glm::mat4& view) { m_shaderProgram.setUniform("view", view); }
    void setProjection(const glm::mat4& proj) { m_shaderProgram.setUniform("proj", proj); }
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
