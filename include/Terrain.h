#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include "Vertex.h"
#include "Shader.h"


namespace fly
{


class Terrain
{
public:
    ~Terrain();
    void generate(int m);
    void draw();
private:
    std::vector<Vertex> m_vertices;
    ShaderProgram m_shader;

    // TEMP
    GLuint texture;
    GLuint vertexBuffer;
    std::vector<GLuint> elements;
    GLuint ebo;
};

}
#endif // TERRAIN_H
