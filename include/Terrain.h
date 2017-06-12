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
    Terrain(ShaderProgram& shader) : m_shader(shader), texture(0), vertexBuffer(0), ebo(0) {}
    ~Terrain();
    void generate(int m);
    void draw();
private:
    std::vector<Vertex> m_vertices;
    ShaderProgram& m_shader;

    // TODO: Move these outside this class (also move shaderprogram) to something like a TextureRenderer or independent classes
    GLuint texture;
    GLuint vertexBuffer;
    std::vector<GLuint> elements;
    GLuint ebo;
};

}
#endif // TERRAIN_H
