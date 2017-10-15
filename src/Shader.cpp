#include "Shader.h"
#include "Log.h"
#include "Utility.h"

#include <fstream>
#include <iterator>
#include <glm/gtc/type_ptr.hpp>

namespace fly
{


ShaderProgram::ShaderProgram() : m_vertexShader(0), m_fragmentShader(0), m_handle(0)
{
    m_handle = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
    if (m_vertexShader)
        glDeleteShader(m_vertexShader);
    if (m_fragmentShader)
        glDeleteShader(m_fragmentShader);
    if (m_handle)
        glDeleteProgram(m_handle);
}

void ShaderProgram::use()
{
    glUseProgram(m_handle);
}


bool ShaderProgram::loadShaderFile(const std::string& path, Shader type)
{
    GLint& shader = type == Shader::Vertex ? m_vertexShader : m_fragmentShader;
    if (shader != 0)
    {
        LOG(Error) << "Shader already loaded." << std::endl;
        return false;
    }

    std::ifstream file(path);
    if (!file.good())
    {
        LOG(Error) << "Error in opening file" << std::endl;
        return false;
    }

    std::string shader_src{std::istreambuf_iterator<char>{file}, {}};

    return loadShaderString(shader_src, type);
}

bool ShaderProgram::loadShaderString(const std::string& shader_src, Shader type)
{
    GLint& shader = type == Shader::Vertex ? m_vertexShader : m_fragmentShader;
    if (shader != 0)
    {
        LOG(Error) << "Shader already loaded." << std::endl;
        return false;
    }

    shader = glCreateShader(type == Shader::Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
    if (shader == 0)
    {
        LOG(Error) << "Creating shader in the GPU failed." << std::endl;
        return false;
    }

    const char* ptr = shader_src.c_str();
    glShaderSource(shader, 1, &ptr, nullptr);
    glCompileShader(shader);
    GLint compile_status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
    char buffer[512];
    glGetShaderInfoLog(shader, 512, NULL, buffer);
    if (compile_status != GL_TRUE)
    {
        LOG(Error) << (type == Shader::Vertex ? "Vertex" : "Fragment") << " shader compilation failed." << std::endl;
        LOG(Error) << "Compiler log:\n" << buffer << std::endl;
        return false;
    }
    else if (std::strlen(buffer) > 0)
    {
        LOG(Debug) << "Compiler log:\n" << buffer << std::endl;
    }

    if (m_vertexShader != 0 && m_fragmentShader != 0) // if both have been supplied
    {
        glAttachShader(m_handle, m_vertexShader);
        glAttachShader(m_handle, m_fragmentShader);
        glLinkProgram(m_handle);
    }

    return ASSERT_GL_ERRORS();
}

bool ShaderProgram::setAttributeFloat(const std::string& name, GLsizei size, GLsizei stride, unsigned int offset)
{
    auto attrib = glGetAttribLocation(m_handle, name.c_str());
    if (attrib == -1)
    {
        LOG(Error) << "Attribute \"" << name << "\" is either reserved or not found" << std::endl;
        return false;
    }
    glVertexAttribPointer(attrib, size, GL_FLOAT, GL_FALSE,
                          stride, reinterpret_cast<void*>(offset));
    glEnableVertexAttribArray(attrib);
    return ASSERT_GL_ERRORS();
}

bool ShaderProgram::setUniform(GLint uniform, float value)
{
    use();
    glUniform1f(uniform, value);
    return ASSERT_GL_ERRORS();
}

bool ShaderProgram::setUniform(GLint uniform, int value)
{
    use();
    glUniform1i(uniform, value);
    return ASSERT_GL_ERRORS();
}

bool ShaderProgram::setUniform(GLint uniform, const glm::mat4& matrix)
{
    use();
    glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(matrix));
    return ASSERT_GL_ERRORS();
}

bool ShaderProgram::setUniform(GLint uniform, const glm::vec3& vec)
{
    use();
    glUniform3fv(uniform, 1, &vec[0]);
    return ASSERT_GL_ERRORS();
}

bool ShaderProgram::setUniform(GLint uniform, bool value)
{
    use();
    glUniform1i(uniform, value);
    return ASSERT_GL_ERRORS();
}



}
