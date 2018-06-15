#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include <string>
#include <vector>
#include <map>
#include <GL/glew.h>
#include <cstdint>

namespace fly
{


class TextureManager
{
public:
    enum { TextureCube = true, };

    ~TextureManager();
    static bool uploadFile(const std::string& name, const std::string& filetype, bool cube = false)
    { return cube ? get().priv_uploadCube(name, filetype) : get().priv_uploadFile(name, filetype); }
    static int  getSampler(const std::string& name) { return get().priv_getSampler(name); }
    static uint generateTexture(const std::string& name = "") { return get().priv_generateTexture(name); }
private:
    TextureManager();

    bool priv_uploadFile(const std::string& name, const std::string& filetype);
    bool priv_uploadCube(const std::string& name, const std::string& filetype);
    int  priv_getSampler(const std::string& name) { return m_samplerMap.at(name); }
    uint priv_generateTexture(const std::string& name);

    static TextureManager& get();

    const uint                  m_maxTextures;
    uint                        m_counter;
    std::vector<GLuint>         m_textures;
    std::map<std::string, int>  m_samplerMap;
};

}

#endif // TEXTUREMANAGER_H
