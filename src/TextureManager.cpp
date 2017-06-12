#include "TextureManager.h"
#include "Log.h"
#include "Utility.h"
#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>

namespace fly
{

bool TextureManager::priv_uploadFile(const std::string& path)
{
    if (m_counter >= GL_MAX_TEXTURE_UNITS)
    {
        LOG(Error) << "Exceeded the maximum texture units available." << std::endl;
        return false;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    sf::Image image;
    glActiveTexture(GL_TEXTURE0 + m_counter);
    glBindTexture(GL_TEXTURE_2D, texture);
    if (!image.loadFromFile(path))
    {
        LOG(Error) << "Couldn't load texture file" << std::endl;
        return false;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.getSize().x, image.getSize().y, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, image.getPixelsPtr());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    bool res;
    if ((res = ASSERT_GL_ERRORS()) == true)
    {
        m_textures.push_back(texture);
        m_samplerMap.emplace(path, m_counter++);
    }
    else
        glDeleteTextures(1, &texture);

    return res;
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
