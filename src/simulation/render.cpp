#include "render.h"

#include <iomanip>
#include <sstream>

#include <SFML/Graphics.hpp>

#include "world.h"

namespace
{
struct BackdropKeyframe
{
    float altitudeMeters;
    sf::Color color;
};

constexpr BackdropKeyframe backdropKeyframes[] = {
    {0.f, sf::Color(72, 170, 240)},
    {15000.f, sf::Color(35, 95, 190)},
    {40000.f, sf::Color(12, 32, 90)},
    {80000.f, sf::Color(4, 8, 25)},
    {100000.f, sf::Color(2, 3, 10)}};

sf::Color blendColors(const sf::Color& from, const sf::Color& to, float amount)
{
    const auto blendChannel = [amount](std::uint8_t first, std::uint8_t second)
    {
        return static_cast<std::uint8_t>(first + (second - first) * amount);
    };

    return sf::Color(blendChannel(from.r, to.r),
                     blendChannel(from.g, to.g),
                     blendChannel(from.b, to.b));
}

sf::Color backdropColorAt(float altitudeMeters)
{
    if (altitudeMeters <= backdropKeyframes[0].altitudeMeters)
    {
        return backdropKeyframes[0].color;
    }

    for (std::size_t index = 1; index < std::size(backdropKeyframes); ++index)
    {
        const BackdropKeyframe& previous = backdropKeyframes[index - 1];
        const BackdropKeyframe& next = backdropKeyframes[index];
        if (altitudeMeters <= next.altitudeMeters)
        {
            const float range = next.altitudeMeters - previous.altitudeMeters;
            const float amount = (altitudeMeters - previous.altitudeMeters) / range;
            return blendColors(previous.color, next.color, amount);
        }
    }

    return backdropKeyframes[std::size(backdropKeyframes) - 1].color;
}

std::string formatAltitude(float meters)
{
    std::ostringstream text;
    text << std::fixed << std::setprecision(1) << meters / 1000.f << " km";
    return text.str();
}
}

void drawSimulationBackdrop(sf::RenderWindow& window, float altitudeMeters)
{
    window.clear(backdropColorAt(altitudeMeters));
}

void drawSimulation(sf::RenderWindow& window,
                    const RocketState& state,
                    SimulationResult result)
{
    sf::RectangleShape ground({40000.f, 40.f});
    ground.setPosition({-20000.f, 0.f});
    ground.setFillColor(sf::Color(40, 104, 74));

    sf::RectangleShape rocket({12.f, 40.f});
    rocket.setOrigin({6.f, 40.f});
    rocket.setPosition({state.horizontalPositionMeters * Rendering::pixelsPerMeter,
                        -state.altitudeMeters * Rendering::pixelsPerMeter});
    rocket.setRotation(sf::degrees(state.angleDegrees));
    rocket.setFillColor(sf::Color(42, 130, 220));

    window.draw(ground);
    window.draw(rocket);
}

void drawSimulationHud(sf::RenderWindow& window,
                       const RocketState& state,
                       SimulationResult result,
                       const sf::Font& font,
                       float speedMultiplier)
{
    sf::Text telemetry(font, "Altitude: " + formatAltitude(state.altitudeMeters) + "\nFuel: " + std::to_string(static_cast<int>(state.fuelMassKg)) + " kg");
    telemetry.setCharacterSize(22);
    telemetry.setString(telemetry.getString() + "\nGoal: 100 km (Karman line)\nVertical speed: " + std::to_string(static_cast<int>(state.verticalVelocityMetersPerSecond)) + " m/s");
    telemetry.setPosition({18.f, 18.f});
    telemetry.setFillColor(sf::Color::White);
    window.draw(telemetry);

    sf::RectangleShape speedButton({140.f, 42.f});
    speedButton.setPosition({640.f, 18.f});
    speedButton.setFillColor(sf::Color(37, 67, 92));
    window.draw(speedButton);

    sf::Text speedLabel(font, std::to_string(static_cast<int>(speedMultiplier)) + "x SPEED", 18);
    const sf::FloatRect labelBounds = speedLabel.getLocalBounds();
    speedLabel.setPosition({640.f + (140.f - labelBounds.size.x) / 2.f - labelBounds.position.x,
                            18.f + (42.f - labelBounds.size.y) / 2.f - labelBounds.position.y});
    window.draw(speedLabel);

    if (result != SimulationResult::Flying)
    {
        const std::string message = result == SimulationResult::Success ? "Space reached - mission success" : "Grounded below target - mission failed";
        sf::Text outcome(font, message + "\nPress R for configuration or Escape to exit", 28);
        outcome.setPosition({45.f, 330.f});
        outcome.setFillColor(result == SimulationResult::Success ? sf::Color(120, 230, 150)
                                                                  : sf::Color(240, 110, 100));
        window.draw(outcome);
    }
}
