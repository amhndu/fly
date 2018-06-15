#ifndef MODEL_H
#define MODEL_H
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <cstdint>
#include "Shader.h"
#include "VertexArrayObject.h"
#include "Renderer.h"
#include "BoundingBox.h"

namespace fly
{


class Model : public Renderer
{
public:
    Model(const std::string& model_path);
    ~Model();
    void  setView(const glm::mat4& view) override { m_shaderProgram.setUniform("view", view); }
    void  setProjection(const glm::mat4& proj) override { m_shaderProgram.setUniform("proj", proj); }
    void  setTransform(const glm::mat4& trans) { m_shaderProgram.setUniform("model", trans); }
    void  setLightDirection(const glm::vec3& dir) { m_shaderProgram.setUniform("light_direction", dir); }
    const AABB& getLocalBounds() { return m_localBounds; }
    void  draw()    override;
    void  rawDraw() override;
    void  darken() { m_monotoneColor = true; }
private:
    bool  m_monotoneColor;

    ShaderProgram m_shaderProgram;

    struct Material
    {
        glm::vec3 ambient_color;
        glm::vec3 diffuse_color;
        glm::vec3 specular_color;
        float     specular_exponent;
    };

    struct Mesh
    {
        std::string name;
        uint        elements_offset;
        uint        elements_size;
        Material&   material;
    };

    std::vector<Material> m_materials;
    std::vector<Mesh>     m_meshes;

    AABB m_localBounds;

    uint m_totalElements;

    VertexArrayObject m_vao;
    GLuint m_vertexBuffer;
    GLuint m_elementBuffer;

};


}

#endif // MODEL_H
