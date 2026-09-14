#include "inputManager.h"

FlightInput InputManager::readFlightInput() const
{
    FlightInput input;
    input.throttle = 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    {
        input.throttle = 0.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        input.steering -= 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        input.steering += 1.f;
    }
    return input;
}