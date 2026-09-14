#pragma once

#include <SFML/Graphics.hpp>
#include "rocket.h"
using namespace sf;

enum class ConfigAction
{
	None,
	Back,
	Start
};

class RocketConfig
{
public:
	RocketConfig();

	void draw(RenderWindow& window) const;
	ConfigAction handleClick(Vector2i position);
	RocketParameters parameters() const;

private:
	Font font;
	bool fontLoaded;
	int dryMass;
	int fuelMass;
	int engineThrust;
	int burnRate;
	int dragCoefficient;
	int launchAngle;
};