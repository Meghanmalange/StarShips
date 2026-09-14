#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Event.hpp>

#include "camera.h"
#include "gameState.h"
#include "inputManager.h"
#include "rocket.h"

enum class SimulationAction
{
    None,
    Configure,
    Exit
};

class Simulation
{
public:
    explicit Simulation(const RocketParameters& parameters);

    void update(float deltaTimeSeconds);
    void draw(sf::RenderWindow& window);
    SimulationAction handleEvent(const sf::Event& event);
    SimulationResult result() const;
    float speedMultiplier() const;

private:
    void reset();

    RocketParameters parameters;
    RocketState state;
    InputManager inputManager;
    Camera camera;
    sf::Font font;
    bool fontLoaded = false;
    int speedIndex = 0;
    SimulationResult simulationResult;
};