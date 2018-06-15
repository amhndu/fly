#include <chrono>
#include <SFML/Window.hpp>
#include <GL/glew.h>
#include <glm/gtx/transform.hpp>
#include <cstdint>
#include "Terrain.h"
#include "Log.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Controller.h"
#include "Airplane.h"
#include "Utility.h"
#include "Sky.h"
#include "ShadowMap.h"
#include "CameraController.h"
#include "ParticleSystem.h"
#include "Sprite.h"
#include "Debug/Box.h"

struct Options
{
    float seed;
    uint  windowWidth;
    uint  windowHeight;
    bool  manualSeed;
    bool  wireframe;
    bool  showHelp;
    bool  fullscreen;
    bool  planeBox;
};

const Options DefaultOptions {0.f, 1024u, 720u, false, false, false, false, false};

void printHelp()
{
    std::cout << "Fly -- A flight simulator" << std::endl
              << "usage: Fly [options...]" << std::endl
              << std::endl
              << "-h   | --help        Print this help text and exit" << std::endl
              << "-w X | wX            Set window width to X (default: "
                << DefaultOptions.windowWidth << ")" << std::endl
              << "-H Y | HY            Set window height to Y (default: "
                << DefaultOptions.windowHeight << ")" << std::endl
              << "-s Z | sZ            Set seed to Z (default: random seed)" << std::endl
              << "-f   | --fullscreen  Set fullscreen mode (default: " << std::boolalpha
                << DefaultOptions.fullscreen << ")" << std::endl
              << "--wireframe          Render in wireframe mode (default: " << std::boolalpha
                << DefaultOptions.wireframe << ")" << std::endl
              << "--plane-box          Draw a bounding box around the plane (default: " << std::boolalpha
                << DefaultOptions.planeBox << ")" << std::endl
              << std::endl;
}

Options processArguments(int argc, char** argv)
{
    using namespace fly;

    std::vector<std::string> arguments;
    Options opts = DefaultOptions;

    for (int i = 1; i < argc; ++i)
        arguments.push_back(argv[i]);

    for (auto i = arguments.begin(); i != arguments.end(); ++i)
    {
        auto&& arg = *i;
        if (arg == "-f" || arg == "--fullscreen")
        {
            opts.fullscreen = true;
            LOG(Info) << "Window set to fullscreen." << std::endl;
        }
        else if (arg == "--wireframe")
        {
            opts.wireframe = true;
            LOG(Info) << "Rendering in wireframe mode." << std::endl;
        }
        else if (arg == "--plane-box")
        {
            opts.planeBox = true;
            LOG(Info) << "Drawing bounding box around plane." << std::endl;
        }
        else if (arg.substr(0, 2) == "-w")
        {
            auto width = arg.substr(2);
            if (width.empty() && std::next(i) != arguments.end())
                width = *++i;
            try
            {
                opts.windowWidth = std::stoi(width);
                LOG(Info) << "Window width set to " << opts.windowWidth << std::endl;
            }
            catch(std::exception error)
            {
                LOG(Error) << "Invalid parameter for window width" << std::endl;
            }
        }
        else if (arg.substr(0, 2) == "-H")
        {
            auto height = arg.substr(2);
            if (height.empty() && std::next(i) != arguments.end())
                height = *++i;
            try
            {
                opts.windowHeight = std::stoi(height);
                LOG(Info) << "Window height set to " << opts.windowHeight << std::endl;
            }
            catch(std::exception error)
            {
                LOG(Error) << "Invalid parameter for window height" << std::endl;
            }
        }
        else if (arg.substr(0, 2) == "-s")
        {
            auto seed = arg.substr(2);
            if (seed.empty() && std::next(i) != arguments.end())
                seed = *++i;
            try
            {
                opts.seed = std::stol(seed);
                opts.manualSeed = true;
                LOG(Info) << "Seed set to " << opts.seed << std::endl;
            }
            catch(std::exception error)
            {
                LOG(Error) << "Invalid parameter for seed" << std::endl;
            }
        }
        else if (arg == "-h" || arg == "--help")
        {
            opts.showHelp = true;
            break;
        }
    }

    return opts;
}

int main(int argc, char** argv)
{
    using namespace fly;

    Log::get().setLogStream(std::cout);
    Log::get().setLevel(Debug);

    std::srand(std::time(nullptr));

    Options opts = processArguments(argc, argv);
    if (opts.showHelp)
    {
        printHelp();
        return EXIT_SUCCESS;
    }

    sf::ContextSettings settings;
    settings.depthBits    = 24;
    settings.stencilBits  = 8;
    settings.antialiasingLevel = 2;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.attributeFlags = sf::ContextSettings::Core;

    sf::VideoMode videoMode {opts.windowWidth, opts.windowHeight};
    uint style =  sf::Style::Close | sf::Style::Resize;
    if (opts.fullscreen)
    {
        style = sf::Style::Fullscreen;
        videoMode = sf::VideoMode::getDesktopMode();
    }
    sf::Window window(videoMode, "OpenGL sandbox", style, settings);
    LOG(Info) << "OpenGL context: " << window.getSettings().majorVersion << '.' <<
                                       window.getSettings().minorVersion << std::endl;
    if (window.getSettings().majorVersion < 3 && window.getSettings().minorVersion < 2)
    {
        LOG(Error) << "Incapable OpenGL context" << std::endl;
    }

    if (opts.fullscreen)
        window.setMouseCursorVisible(false);


    // GLEW Init
    glewExperimental = GL_TRUE;
    glewInit();

    // GLEW bug where glewInit() sets GL_INVALID_ENUM
    glGetError();

    TextureManager::uploadFile("terrain_lookup", ".png");
    TextureManager::uploadFile("TropicalSunnyDay/TropicalSunnyDay", ".png",
                               TextureManager::TextureCube);
    TextureManager::uploadFile("flare", ".png");

    // The default projection matrix
    glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f),
                                                   1.f * window.getSize().x / window.getSize().y,
                                                   0.05f, 50.0f);

    Terrain terrain(15, 15);
    Airplane aircraft;
    ShadowMap shadowMap(aircraft);
    Sky sky;
    ParticleSystem particles;

    auto& terrainRenderer  = terrain.getRenderer();
    auto& aircraftRenderer = aircraft.getRenderer();

    terrain.generate(opts.manualSeed ? opts.seed :
                     std::rand() % 1000 + 1.f * std::rand() / RAND_MAX);


    particles.getRenderer().setProjection(projection_matrix);
    aircraftRenderer.       setProjection(projection_matrix);
    terrainRenderer.        setProjection(projection_matrix);
    sky.                    setProjection(projection_matrix);

    aircraftRenderer.setLightDirection({-0.3f, 0.3f, 0.9f});
    terrainRenderer.setLightDirection({-0.3, 0.3, 0.9});
    shadowMap.setLightDirection({-0.3f, 0.3f, 0.9f});


    Camera camera(aircraft.getPosition(),                // Start position
                  aircraft.getForwardDirection(),       // Direction
                  aircraft.getUpDirection(),           // Up
                  aircraft);

    CameraController mouse_camera(window, camera);

    std::unique_ptr<Box> box;
    if (opts.planeBox)
    {
        box.reset(new Box());
        box->setProjection(projection_matrix);
    }

    // Set up input callbacks
    Controller controller(window);
    controller.setCallback(Controller::RollLeft,     std::bind(&Airplane::roll,    &aircraft, -1));
    controller.setCallback(Controller::RollRight,    std::bind(&Airplane::roll,    &aircraft, +1));
    controller.setCallback(Controller::ElevatorUp,   std::bind(&Airplane::elevate, &aircraft, -1));
    controller.setCallback(Controller::ElevatorDown, std::bind(&Airplane::elevate, &aircraft, +1));
    controller.setCallback(Controller::ThrustUp,     std::bind(&Airplane::throttle,&aircraft, +1));
    controller.setCallback(Controller::ThrustDown,   std::bind(&Airplane::throttle,&aircraft, -1));
    controller.registerRotate([&](float x, float y){ camera.rotate(x, y); });

    // GL setup
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    /* Wireframe mode */
    if (opts.wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnable(GL_BLEND);

    auto main_frame_buffer = FrameBuffer::Builder(window.getSize().x, window.getSize().y)
                                .attachColor0("mainFrameBuffer")
                                .attachDepthTexture()
                                .build();

    particles.getRenderer().useFrameBuffer(FrameBuffer::Builder(window.getSize().x, window.getSize().y)
                                            .attachColor0("particlesFrameBuffer")
                                            .attachDepthTexture(main_frame_buffer.getDepthBuffer())
                                            .build());
    Sprite proxySprite;
    proxySprite.setTexture(TextureManager::getSampler("mainFrameBuffer"));

    int viewport_save[4];
    glGetIntegerv(GL_VIEWPORT, viewport_save);

    auto prev_time = std::chrono::steady_clock::now();
    const std::chrono::steady_clock::duration frame_period(std::chrono::milliseconds(1000 / 60));
    const float frame_period_seconds = std::chrono::duration<float>(frame_period).count();
    sf::Event event;

    // Perhaps use a state system here...
    bool running = true;
    bool focus   = true;
    bool paused  = false;
    bool crashed = false;
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
            else if (focus && event.type == sf::Event::KeyReleased
                    && event.key.code == sf::Keyboard::F4)
                paused = !paused;
            else if (focus)
                mouse_camera.passEvent(event);

        }

        auto now = std::chrono::steady_clock::now();
        while (focus && now - prev_time > frame_period)
        {
            controller.takeInput(frame_period_seconds);

            if (!paused)
            {
                if (!crashed)
                {
                    aircraft.update(frame_period_seconds);
                    terrain.setCenter(aircraft.getPosition());
                }
                particles.update(frame_period_seconds);
            }

            mouse_camera.update(frame_period_seconds);
            camera.updateView(frame_period_seconds);

            if (camera.viewChanged())
            {
                glm::mat4 view = camera.getView();
                particles.getRenderer().setView(view);
                aircraftRenderer.setView(view);
                terrainRenderer.setView(view);
                sky.setView(view);
                if (opts.planeBox)
                    box->setView(view);
            }

            auto&& light_space = shadowMap.update();
            terrainRenderer.setLightSpace(light_space);
            glViewport(viewport_save[0], viewport_save[1], viewport_save[2], viewport_save[3]);

            auto boundingBox = glm::translate(aircraft.getModel(),
                                              aircraft.getLocalBounds().position);
            if (!crashed && terrain.above({aircraft.getLocalBounds().dimensions, boundingBox}))
            {
                aircraft.crash();
                crashed = true;
                particles.reset(4601);
                particles.addEmitter(Emitter::createUniformCone(650 * 1.5f, aircraft.getPosition(),
                            glm::vec3{0.001f, 0.001f, 0.0020f}, {0.f, 0.f, 0.023f},
                            4.f, 5.f, 1.00f, 3.00f, PI / 4.f, 600));
                particles.addEmitter(Emitter::createExplosion(aircraft.getPosition(),
                                                              {0.f, 0.f, 1.f},
                                                              {0.f, 0.f, -0.05f * 1.5f * 1.5f},
                                                              0.08f * 1.5f));
                particles.addUpdater(Updater::basicPhysics);
                particles.addUpdater(Updater::lifeUpdater);
                particles.addUpdater(Updater::fireColor);
                particles.addUpdater(Updater::createLinearSize(0.001f, 0.020f));
                particles.getRenderer().setBlending(Additive);
            }

            main_frame_buffer.bind();
            glClear(GL_DEPTH_BUFFER_BIT);
             if (opts.wireframe)
                glClear(GL_COLOR_BUFFER_BIT);
            aircraftRenderer.draw();
            terrainRenderer.draw();
            if (opts.planeBox)
            {
                box->setTransform(glm::scale(boundingBox, aircraft.getLocalBounds().dimensions));
                box->draw();
            }
            sky.draw();
            particles.getRenderer().draw(main_frame_buffer);

            FrameBuffer::bindDefault();
            proxySprite.draw();
            window.display();

            prev_time += frame_period;
        }
        // For portability, as MinGW's this_thread::sleep_for is broken
        sf::sleep(sf::seconds(1.f / 60.f));
    }

    window.close();
    return EXIT_SUCCESS;
}
