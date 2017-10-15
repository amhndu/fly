#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

namespace fly
{


enum class Shader
{
    Vertex,
    Fragment
};

class ShaderProgram
{
public:
    ShaderProgram();
    ~ShaderProgram();

    ShaderProgram(const ShaderProgram& other) = delete;
    ShaderProgram& operator= (const ShaderProgram& other) = delete;

    bool loadShaderFile(const std::string& path, Shader type);
    bool loadShaderString(const std::string& shader_src, Shader type);
    void use();

    bool setAttributeFloat(const std::string& name, GLsizei size,
                           GLsizei stride, unsigned int offset);

    GLint inline getUniformLocation(const std::string& name)
    { return glGetUniformLocation(m_handle, name.c_str()); }

    bool setUniform(GLint uniform, float value);
    bool setUniform(GLint uniform, const glm::mat4& matrix);
    bool setUniform(GLint uniform, const glm::vec3& vec);
    bool setUniform(GLint uniform, int value);
    bool setUniform(GLint uniform, bool value);
    template <typename T>
    bool inline setUniform(const std::string& name, const T& value)
    { return setUniform(getUniformLocation(name), value); }

private:
    GLint m_vertexShader;
    GLint m_fragmentShader;
    GLint m_handle;
};


}
#endif // SHADERPROGRAM_H
