#ifndef TERRAINRENDERER_H
#define TERRAINRENDERER_H

#include <GL/glew.h>
#include <vector>
#include <map>
#include <cstdint>
#include "Shader.h"
#include "Vertex.h"

namespace fly
{

class TerrainRenderer
{
public:
    TerrainRenderer(ShaderProgram& shader, int radius, int detail);
    ~TerrainRenderer();
    void reset(int radius, int detail);
    void updateChunk(int chunk_x, int chunk_y, int coord_x, int coord_y, const std::vector<float>& chunk_heights);
    void draw();
private:
    ShaderProgram& m_shaderProgram;

    GLuint m_vertexArrayObject;
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
