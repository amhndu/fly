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
// #ifndef NDEBUG
//     if (!no_errors)
//         throw std::runtime_error(std::string("In `") + caller + "' OpenGL error: " + std::to_string(err));
// #endif
    return no_errors;
}

}
