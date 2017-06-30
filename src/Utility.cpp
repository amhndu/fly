#include "Utility.h"
#include "Log.h"
#include <GL/glew.h>

namespace fly
{

bool assertGLErrorsImpl(const char* caller)
{
    GLenum err;
    bool no_errors = (err = glGetError()) == GL_NO_ERROR;
    while (err != GL_NO_ERROR)
    {
        LOG(Error) << "In `" << caller <<"' OpenGL error: " << err << std::endl;
        err = glGetError();
    }
    assert(no_errors);
    return no_errors;
}

}
