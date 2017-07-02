#include <chrono>
#include <SFML/Window.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include "Terrain.h"
#include "Log.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Controller.h"
#include "Airplane.h"

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
        LOG(Info) << "OpenGL context: " << c.majorVersion << '.' << c.minorVersion << std::endl;
        if (c.majorVersion < 3 && c.minorVersion < 2)
        {
            LOG(Error) << "Incapable OpenGL context" << std::endl;
        }
    }

    // window.setMouseCursorGrabbed(true);
    // window.setMouseCursorVisible(false);

    // GLEW Init
    glewExperimental = GL_TRUE;
    glewInit();

    TextureManager::uploadFile("resources/texture.png");

    // The default projection matrix
    glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f),
                                                   1.f * window.getSize().x / window.getSize().y,
                                                   0.05f, 50.0f);
    Terrain terrain(10, 20);
    terrain.generate(5.14f);    // a constant seed for now
    terrain.setProjection(projection_matrix);

    Airplane aircraft;
    aircraft.setProjection(projection_matrix);

    Camera camera(aircraft.getPosition(),                // Start position
                  aircraft.getForwardDirection(),       // Direction
                  aircraft.getUpDirection(),           // Up
                  aircraft);

    // Set up input callbacks
    Controller controller(window);
    controller.setCallback(Controller::RollLeft,     std::bind(&Airplane::roll,    &aircraft, -1));
    controller.setCallback(Controller::RollRight,    std::bind(&Airplane::roll,    &aircraft, +1));
    controller.setCallback(Controller::ElevatorUp,   std::bind(&Airplane::elevate, &aircraft, -1));
    controller.setCallback(Controller::ElevatorDown, std::bind(&Airplane::elevate, &aircraft, +1));

    // GL setup
    glEnable(GL_DEPTH_TEST);

    /* Wireframe mode
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    //*/

    auto prev_time = std::chrono::steady_clock::now();
    const std::chrono::steady_clock::duration frame_period(std::chrono::milliseconds(1000/60));
    const float frame_period_seconds = std::chrono::duration<float>(frame_period).count();
    sf::Event event;
    bool running = true;
    bool focus = true;
    while (running)
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed ||
               (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape))
                running = false;
            else if (event.type == sf::Event::GainedFocus)
            {
                focus = true;
                prev_time = std::chrono::steady_clock::now();
            }
            else if (event.type == sf::Event::LostFocus)
                focus = false;
            /*else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::G)
                terrain.generate(rand() / 65536.f);*/
        }

        auto now = std::chrono::steady_clock::now();
        while (focus && now - prev_time > frame_period)
        {
            controller.takeInput(frame_period_seconds);

            aircraft.update(frame_period_seconds);
            terrain.setCenter(aircraft.getPosition());
            camera.updateView(frame_period_seconds);

            glClearColor(0.04f, 0.04f, 0.04f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if (camera.viewChanged())
            {
                glm::mat4 view = camera.getView();
                terrain.setView(view);
                aircraft.setView(view);
            }
            terrain.draw();
            aircraft.draw();

            window.display();

            prev_time += frame_period;
        }
        sf::sleep(sf::seconds(1.f/60.f));  // MinGW's this_thread::sleep_for doesn't work for some reason
    }

    window.close();
    return 0;
}
