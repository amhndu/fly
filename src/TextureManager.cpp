#include "TextureManager.h"
#include "Log.h"
#include "Utility.h"
#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>
#include <cstdint>

namespace fly
{

static uint GetMaxTextureUnits()
{
    static int result = -1;
    if (result == -1)
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &result);
    return static_cast<uint>(result);
}


TextureManager::TextureManager() :
    m_maxTextures(GetMaxTextureUnits()),
    m_counter(0)
{}

bool TextureManager::priv_uploadFile(const std::string& name, const std::string& filetype)
{
    if (m_counter >= m_maxTextures)
    {
        LOG(Error) << "Exceeded the maximum texture units available." << std::endl;
        return false;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0 + m_counter);
    glBindTexture(GL_TEXTURE_2D, texture);
    sf::Image image;
    if (!image.loadFromFile("resources/" + name + filetype))
    {
        LOG(Error) << "Couldn't load texture file" << std::endl;
        return false;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, image.getPixelsPtr());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    bool res;
    if ((res = ASSERT_GL_ERRORS()) == true)
    {
        m_textures.push_back(texture);
        m_samplerMap.emplace(name, m_counter++);
        LOG(Debug) << "Uploaded texture " << name << std::endl;
    }
    else
        glDeleteTextures(1, &texture);

    return res;
}

bool TextureManager::priv_uploadCube(const std::string& name, const std::string& filetype)
{
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    int target = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
    for (const std::string& suffix : {"rt", "lf", "up", "dn", "bk", "ft"})
    {
        sf::Image image;
        if (!image.loadFromFile("resources/" + name + "_" + suffix + filetype))
        {
            LOG(Error) << "Failed to open texture for sky box: " << name + "_" + suffix << std::endl;
            return false;
        }

        glTexImage2D(target, 0, GL_RGB, image.getSize().x, image.getSize().y, 0, GL_RGBA,
                        GL_UNSIGNED_BYTE, image.getPixelsPtr());
        ++target;
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    bool res;
    if ((res = ASSERT_GL_ERRORS()) == true)
    {
        m_textures.push_back(texture);
        m_samplerMap.emplace(name, m_counter++);
    }
    else
        glDeleteTextures(1, &texture);

    return res;
}

uint TextureManager::priv_generateTexture(const std::string& name)
{
    GLuint texture;
    glGenTextures(1, &texture);
    LOG(Debug) << "Generated texture " << name << " id " << texture << " sampler " << m_counter << std::endl;
    m_textures.push_back(texture);
    glActiveTexture(GL_TEXTURE0 + m_counter);
    if (!name.empty())
    {
        m_samplerMap.emplace(name, m_counter);
    }
    ++m_counter;
    return texture;
}

TextureManager::~TextureManager()
{
    glDeleteTextures(m_textures.size(), m_textures.data());
}

TextureManager& TextureManager::get()
{
    static TextureManager instance;
    return instance;
}


}
