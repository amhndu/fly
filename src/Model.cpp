#include <glm/gtc/matrix_transform.hpp>
#include <OBJ_Loader.h>
#include <unordered_map>
#include "Model.h"
#include "Log.h"
#include "Utility.h"

namespace fly
{

Model::Model(const std::string& model_path)
{
    m_vao.bind();

    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glGenBuffers(1, &m_elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBuffer);

    if (m_shaderProgram.loadShaderFile("shaders/model.vert", Shader::Vertex))
    {
        LOG(Info) << "Loaded Vertex shader" << std::endl;
    }
    if (m_shaderProgram.loadShaderFile("shaders/model.frag", Shader::Fragment))
    {
        LOG(Info) << "Loaded Fragment shader" << std::endl;
    }

    objl::Loader loader;
    if (loader.LoadFile(model_path))
    {
        glBufferData(GL_ARRAY_BUFFER, loader.LoadedVertices.size() * sizeof(objl::Vertex),
                     loader.LoadedVertices.data(), GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, loader.LoadedIndices.size() * sizeof(unsigned int),
                     loader.LoadedIndices.data(), GL_STATIC_DRAW);

        m_shaderProgram.use();
        m_shaderProgram.setAttributeFloat("position",  3, sizeof(objl::Vertex), offsetof(objl::Vertex, Position));
        m_shaderProgram.setAttributeFloat("normal",    3, sizeof(objl::Vertex), offsetof(objl::Vertex, Normal));
        m_shaderProgram.setAttributeFloat("texcoords", 2, sizeof(objl::Vertex), offsetof(objl::Vertex, TextureCoordinate));
        // TODO Remove texcoords (since I'm not using it)

        std::unordered_map<std::string, std::size_t> materials_map;

        for (const auto& m : loader.LoadedMaterials)
        {
            materials_map[m.name] = m_materials.size();
            m_materials.push_back({
                            {m.Ka.X, m.Ka.Y, m.Ka.Z},
                            {m.Kd.X, m.Kd.Y, m.Kd.Z},
                            //{m.Ks.X, m.Ks.Y, m.Ks.Z},
                            //m.Ns
                           });
        }

        uint offset = 0;
        for (const auto& mesh : loader.LoadedMeshes)
        {
            m_meshes.push_back({mesh.MeshName, offset, static_cast<uint>(mesh.Indices.size()),
                                m_materials.at(materials_map[mesh.MeshMaterial.name])});
            offset += mesh.Indices.size();
        }
    }
    else
        LOG(Error) << "Error in opening model file: " << model_path << std::endl;

    m_vao.unbind();
    ASSERT_GL_ERRORS();
}

Model::~Model()
{
    glDeleteBuffers(1, &m_vertexBuffer);
    glDeleteBuffers(1, &m_elementBuffer);
}

void Model::draw()
{
    m_vao.bind();
    m_shaderProgram.use();
    for (auto& mesh : m_meshes)
    {
        m_shaderProgram.setUniform("ambient_color", mesh.material.ambient_color);
        m_shaderProgram.setUniform("diffuse_color", mesh.material.diffuse_color);
        //m_shaderProgram.setUniform("specular_exponent", mesh.material.specular_exponent);
        //m_shaderProgram.setUniform("specular_color", mesh.material.specular_color);
        glDrawElements(GL_TRIANGLES, mesh.elements_size, GL_UNSIGNED_INT,
                       reinterpret_cast<void*>(mesh.elements_offset * sizeof(unsigned int)));
    }
    m_vao.unbind();
}



}