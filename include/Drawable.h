#ifndef FLY_DRAWABLE_H
#define FLY_DRAWABLE_H
#include <glm/mat4x4.hpp>

namespace fly
{

class Drawable
{
public:
    // Setup the specific shader and draw
    virtual void  draw()    = 0;
    // Just draw, with whatever shader's bound. Used to create the shadow map.
    virtual void  rawDraw() = 0; // I really dig that rhyme
    // Get the model transformation matrix, default is the identity matrix
    virtual const glm::mat4 getModel() { return glm::mat4(1.f); };
};

}
#endif //FLY_DRAWABLE_H
