#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>

#include "gameState.h"
#include "rocket.h"

void drawSimulationBackdrop(sf::RenderWindow& window, float altitudeMeters);

void drawSimulation(sf::RenderWindow& window,
					const RocketState& state,
					SimulationResult result);

void drawSimulationHud(sf::RenderWindow& window,
					   const RocketState& state,
					   SimulationResult result,
					   const sf::Font& font,
					   float speedMultiplier);
