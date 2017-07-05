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
#include "Utility.h"

struct Options
{
    bool  fullscreen;
    uint  windowWidth;
    uint  windowHeight;
    bool  manualSeed;
    float seed;
    bool  showHelp;
};

const Options DefaultOptions {false, 800u, 600u, false, 0.f, false};

void printHelp()
{
    std::cout << "Fly -- A flight simulator" << std::endl
              << "usage: Fly [options...]" << std::endl
              << std::endl
              << "-h   | --help        Print this help text and exit" << std::endl
              << "-w X | wX            Set window width to X (default: " << DefaultOptions.windowWidth << ")" << std::endl
              << "-H Y | HY            Set window height to Y (default: " << DefaultOptions.windowHeight << ")" << std::endl
              << "-s Z | sZ            Set seed to Z (default: random seed)" << std::endl
              << "-f   | --fullscreen  Set fullscreen mode (default: " << std::boolalpha
                                                                     << DefaultOptions.fullscreen << ")" << std::endl
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
            LOG(Info) << "Window set to fullscreen" << std::endl;
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
    settings.depthBits = 24;
    settings.stencilBits = 8;
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

    TextureManager::uploadFile("resources/texture.png");

    // The default projection matrix
    glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f),
                                                   1.f * window.getSize().x / window.getSize().y,
                                                   0.05f, 50.0f);
    Terrain terrain(15, 15);
    terrain.generate(opts.manualSeed ? opts.seed : std::rand() % 1000 + 1.f * std::rand() / RAND_MAX);
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

            glClearColor(100.f / 255.f, 250.f / 255.f, 255.f / 255.f, 1.0f);
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
    return EXIT_SUCCESS;
}
