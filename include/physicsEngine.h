#pragma once

#include "rocket.h"
#include "gameState.h"

SimulationResult evaluateFlight(const RocketParameters& parameters, const RocketState& state);

struct PhysicsInput
{
	float throttle = 0.f;
	float steering = 0.f;
};

void updateRocketPhysics(const RocketParameters& parameters,
						 RocketState& state,
						 const PhysicsInput& input,
						 float deltaTimeSeconds);
