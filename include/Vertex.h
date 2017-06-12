#ifndef VERTEX_H
#define VERTEX_H
#include <GL/glew.h>


namespace fly
{

struct Vertex
{
    GLfloat position[3];
    GLfloat texcoords[2];
};


}

#endif // VERTEX_H
