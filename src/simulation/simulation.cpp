#include "simulation.h"

#include <algorithm>

#include "physicsEngine.h"
#include "render.h"
#include "world.h"

namespace
{
constexpr float speedOptions[] = {1.f, 5.f, 10.f};
constexpr float physicsStepSeconds = 1.f / 120.f;
}

Simulation::Simulation(const RocketParameters& parameters)
    : parameters(parameters), simulationResult(SimulationResult::Flying)
{
    fontLoaded = font.openFromFile("C:/Windows/Fonts/seguisb.ttf");
    if (!fontLoaded)
    {
        fontLoaded = font.openFromFile("C:/Windows/Fonts/arial.ttf");
    }
    reset();
}

void Simulation::reset()
{
    state = {};
    state.fuelMassKg = parameters.fuelMassKg;
    state.angleDegrees = parameters.launchAngleDegrees;
    speedIndex = 0;
    simulationResult = SimulationResult::Flying;
    camera.follow(state.horizontalPositionMeters, state.altitudeMeters);
}

void Simulation::update(float deltaTimeSeconds)
{
    if (simulationResult != SimulationResult::Flying)
    {
        return;
    }

    float remainingTime = deltaTimeSeconds * speedMultiplier();
    while (remainingTime > 0.f && simulationResult == SimulationResult::Flying)
    {
        const float step = std::min(remainingTime, physicsStepSeconds);
        const FlightInput flightInput = inputManager.readFlightInput();
        state.angleDegrees = std::clamp(state.angleDegrees + flightInput.steering * 25.f * step,
                                        -30.f, 30.f);
        updateRocketPhysics(parameters, state, {flightInput.throttle, flightInput.steering}, step);
        remainingTime -= step;

        simulationResult = evaluateFlight(parameters, state);
    }
    camera.follow(state.horizontalPositionMeters, state.altitudeMeters);
}

void Simulation::draw(sf::RenderWindow& window)
{
    drawSimulationBackdrop(window, state.altitudeMeters);
    window.setView(camera.view());
    drawSimulation(window, state, simulationResult);
    window.setView(window.getDefaultView());
    if (fontLoaded)
    {
        drawSimulationHud(window, state, simulationResult, font, speedMultiplier());
    }
}

SimulationAction Simulation::handleEvent(const sf::Event& event)
{
    if (event.is<sf::Event::MouseButtonPressed>())
    {
        const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>();
        const sf::FloatRect speedButton({640.f, 18.f}, {140.f, 42.f});
        if (mousePressed->button == sf::Mouse::Button::Left &&
            speedButton.contains({static_cast<float>(mousePressed->position.x),
                                  static_cast<float>(mousePressed->position.y)}))
        {
            speedIndex = (speedIndex + 1) % 3;
        }
    }

    if (event.is<sf::Event::KeyPressed>())
    {
        const auto* keyPressed = event.getIf<sf::Event::KeyPressed>();
        if (keyPressed->code == sf::Keyboard::Key::R)
        {
            return SimulationAction::Configure;
        }
        if (keyPressed->code == sf::Keyboard::Key::Tab)
        {
            speedIndex = (speedIndex + 1) % 3;
        }
        if (keyPressed->code == sf::Keyboard::Key::Escape)
        {
            return SimulationAction::Exit;
        }
    }
    return SimulationAction::None;
}

SimulationResult Simulation::result() const
{
    return simulationResult;
}

float Simulation::speedMultiplier() const
{
    return speedOptions[speedIndex];
}
