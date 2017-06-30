#ifndef VERTEXARRAYOBJECT_H
#define VERTEXARRAYOBJECT_H
#include <GL/glew.h>

namespace fly
{

// Simple wrapper for a vao
class VertexArrayObject
{
public:
    VertexArrayObject() { glGenVertexArrays(1, &m_object); }
    ~VertexArrayObject() { glDeleteVertexArrays(1, &m_object); }
    void bind() { glBindVertexArray(m_object); }
    void unbind() {/* glBindVertexArray(0)*/ }

    VertexArrayObject(const VertexArrayObject& other) = delete;
    const VertexArrayObject operator=(const VertexArrayObject& other) = delete;
private:
    GLuint m_object;
};

}
#endif // VERTEXARRAYOBJECT_H
