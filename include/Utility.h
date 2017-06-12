#ifndef UTILITY_H
#define UTILITY_H

#define ASSERT_GL_ERRORS() assertGLErrorsImpl(__func__)

namespace fly
{

bool assertGLErrorsImpl(const char* caller);

}


#endif // UTILITY_H
