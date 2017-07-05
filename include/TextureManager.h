#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include <string>
#include <vector>
#include <map>
#include <GL/glew.h>

namespace fly
{

class TextureManager
{
public:
    TextureManager() : m_counter(0) {}
    ~TextureManager();
    static bool uploadFile(const std::string& path) { return get().priv_uploadFile(path); }
    static int  getSampler(const std::string& path) { return get().priv_getSampler(path); }
private:
    bool priv_uploadFile(const std::string& path);
    int  priv_getSampler(const std::string& path) { return m_samplerMap.at(path); }
    static TextureManager& get();
    unsigned int m_counter;
    std::vector<GLuint> m_textures;
    std::map<std::string, int> m_samplerMap;
};

}

#endif // TEXTUREMANAGER_H
