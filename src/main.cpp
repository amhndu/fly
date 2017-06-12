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
#include "Camera.h"

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
    {
        auto c = window.getSettings();
        LOG(Debug) << "OpenGL version: " << c.majorVersion << '.' << c.minorVersion << std::endl;
    }

    window.setMouseCursorGrabbed(true);
    window.setMouseCursorVisible(false);

    // GLEW Init
    glewExperimental = GL_TRUE;
    glewInit();

    // Vertex Array Object to store the attributes/shaders config
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    TextureManager::uploadFile("texture.png");
    ShaderProgram shader;
    Camera camera(shader,
                  {-1.0f, 0.3f, -1.0f},
                  glm::normalize(glm::vec3{0.8f, -0.1f, 0.6f}),
                  glm::vec3(0.0f, 1.0f, 0.0f),
                  glm::perspective(glm::radians(45.0f), 1.f * window.getSize().x / window.getSize().y, 0.05f, 10.0f),
                  window);
    Terrain terrain(shader);
    terrain.generate(50);

    glEnable(GL_DEPTH_TEST);

    auto prev_time = std::chrono::steady_clock::now();
    const std::chrono::steady_clock::duration frame_period(std::chrono::milliseconds(1000/60));
    const float frame_period_seconds = std::chrono::duration<float>(frame_period).count();
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

        auto now = std::chrono::steady_clock::now();
        while (now - prev_time > frame_period)
        {
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            camera.updateView(frame_period_seconds);
            terrain.draw();

            window.display();

            prev_time += frame_period;
        }
        sf::sleep(sf::seconds(1.f/60.f));  // MinGW's this_thread::sleep_for doesn't work for some reason
    }

    glDeleteVertexArrays(1, &vao);
    window.close();
    return 0;
}
