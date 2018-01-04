#ifndef UTILITY_H
#define UTILITY_H
#include <iostream>
#include <glm/glm.hpp>

#define ASSERT_GL_ERRORS() assertGLErrorsImpl(__func__)

namespace fly
{

const float PI = 3.1415926535;

bool assertGLErrorsImpl(const char* caller);

template <typename T>
constexpr inline T sq(T val)
{
    return val * val;
}

template <typename T>
constexpr inline T sign(T x)
{
    return x != 0 ? x / std::abs(x) : 0;
}

template <typename T, typename U>
inline T lerp(T t, U&& a, U&& b)
{
    return a * (1 - t) + b * t;
}

inline glm::vec3 component_wise_apply(const glm::vec3& a, const glm::vec3& b,
                                    const float& (*f)(const float&, const float&))
{
    return {f(a.x, b.x), f(a.y, b.y), f(a.z, b.z)};
}

inline std::ostream& operator<< (std::basic_ostream<char>& out, const glm::vec3& vec)
{
    out << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")" << "; ";
    return out;
}


}


#endif // UTILITY_H
