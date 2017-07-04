#ifndef VERTEX_H
#define VERTEX_H
#include <GL/glew.h>


namespace fly
{

struct Vertex
{
    GLfloat position[3];
    GLfloat normal[3];
    GLfloat color;
};


}

#endif // VERTEX_H
