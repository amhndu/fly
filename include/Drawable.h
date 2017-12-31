#ifndef FLY_DRAWABLE_H
#define FLY_DRAWABLE_H
#include <glm/mat4x4.hpp>

namespace fly
{

// I made this class primarily to be used in ShadowMap but it turns out,
// only doing it with the Airplane is sufficient.
// This interface still exists because.. I don't to throw away the effort

class Drawable
{
public:
    virtual ~Drawable() {};
    // Setup the specific shader and draw
    virtual void  draw()    = 0;
    // Just draw, with whatever shader's bound. Used to create the shadow map.
    virtual void  rawDraw() = 0; // I really dig that rhyme
    // Get the model transformation matrix, default is the identity matrix
    virtual const glm::mat4 getModel() { return glm::mat4(1.f); };
};

}
#endif //FLY_DRAWABLE_H
