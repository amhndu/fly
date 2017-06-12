#include <iostream>
#include <cmath>
#include <chrono>
#include <SFML/Window.hpp>
#include <SFML/Graphics/Image.hpp>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Terrain.h"
#include "Log.h"
#include "TextureManager.h"

int main()
{
    using namespace fly;

    Log::get().setLogStream(std::cout);
    Log::get().setLevel(Debug);

    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 2;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.attributeFlags = sf::ContextSettings::Core;
    sf::Window window(sf::VideoMode(800, 600), "OpenGL sandbox", sf::Style::Default, settings);
    auto c = window.getSettings();
    LOG(Debug) << "OpenGL version: " << c.majorVersion << '.' << c.minorVersion << std::endl;

    // GLEW Init
    glewExperimental = GL_TRUE;
    glewInit();

    // Vertex Array Object to store the attributes/shaders config
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    TextureManager::uploadFile("texture.png");
    Terrain terrain;
    terrain.generate(25);

    glEnable(GL_DEPTH_TEST);

    sf::Clock timer;
    sf::Event event;
    bool running = true;
    while (running)
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed ||
               (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape))
                running = false;
        }

        if (timer.getElapsedTime() > sf::milliseconds(1000/60))
        {
            // Clear the screen to black
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            terrain.draw();
            window.display();

            timer.restart();
        }
        sf::sleep(sf::seconds(1.f/60.f));
    }

    glDeleteVertexArrays(1, &vao);
    window.close();
    return 0;
}
