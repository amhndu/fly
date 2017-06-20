#ifndef UTILITY_H
#define UTILITY_H
#include <iostream>
#include <glm/glm.hpp>

#define ASSERT_GL_ERRORS() assertGLErrorsImpl(__func__)

namespace fly
{

bool assertGLErrorsImpl(const char* caller);

template <typename T>
T sq(T val)
{
    return val * val;
}


inline std::ostream& operator<< (std::ostream& out, const glm::vec3& vec)
{
    out << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")" << "; ";
    return out;
}


}


#endif // UTILITY_H
