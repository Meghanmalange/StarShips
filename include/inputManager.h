#pragma once

#include <SFML/Window/Keyboard.hpp>

struct FlightInput
{
	float throttle = 0.f;
	float steering = 0.f;
};

class InputManager
{
public:
	FlightInput readFlightInput() const;
};
