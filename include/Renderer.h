#ifndef FLY_RENDERER_H
#define FLY_RENDERER_H
#include <glm/mat4x4.hpp>

namespace fly
{

// Not actually used...
class Renderer
{
public:
    virtual ~Renderer() {}
    virtual void  draw()    = 0;
    // Just draw, with whatever shader's bound. Used to create the shadow map.
    virtual void  rawDraw() = 0; // I really dig that rhyme

    virtual void setProjection(const glm::mat4& proj) = 0;
    virtual void setView(const glm::mat4& view) = 0;
};

}
#endif //FLY_DRAWABLE_H
