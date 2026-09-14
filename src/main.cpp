#include <SFML/Graphics.hpp>
#include <algorithm>
#include <memory>

#include "simulation.h"
#include "ui/mainMenu.h"
#include "ui/rocketConfig.h"
using namespace sf;


enum class Screen
{
    MainMenu,
    RocketConfig,
    Simulation
};

int main()
{
    RenderWindow window(
        VideoMode({800, 800}),
        "StarShips"
    );
    MainMenu mainMenu;
    RocketConfig rocketConfig;
    std::unique_ptr<Simulation> simulation;
    Screen currentScreen = Screen::MainMenu;
    Clock clock;

    while (window.isOpen())
    {
        const float deltaTime = std::min(clock.restart().asSeconds(), 0.05f);
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
            {
                window.close();
            }

            if (const auto* mousePressed = event->getIf<Event::MouseButtonPressed>())
            {
                if (mousePressed->button == Mouse::Button::Left)
                {
                    if (currentScreen == Screen::MainMenu &&
                        mainMenu.startClicked(mousePressed->position))
                    {
                        currentScreen = Screen::RocketConfig;
                    }
                    else if (currentScreen == Screen::RocketConfig)
                    {
                        const ConfigAction action = rocketConfig.handleClick(mousePressed->position);
                        if (action == ConfigAction::Back)
                        {
                            currentScreen = Screen::MainMenu;
                        }
                        else if (action == ConfigAction::Start)
                        {
                            simulation = std::make_unique<Simulation>(rocketConfig.parameters());
                            currentScreen = Screen::Simulation;
                        }
                    }
                }
            }

            if (currentScreen == Screen::Simulation && simulation)
            {
                const SimulationAction action = simulation->handleEvent(*event);
                if (action == SimulationAction::Configure)
                {
                    currentScreen = Screen::RocketConfig;
                }
                else if (action == SimulationAction::Exit)
                {
                    window.close();
                }
            }
        }

        if (currentScreen == Screen::MainMenu)
        {
            window.clear();
            mainMenu.draw(window);
        }
        else if (currentScreen == Screen::RocketConfig)
        {
            rocketConfig.draw(window);
        }
        else if (simulation)
        {
            simulation->update(deltaTime);
            simulation->draw(window);
        }

        window.display();
    }

    return 0;
}