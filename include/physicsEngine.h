#pragma once

#include "rocket.h"

struct PhysicsInput
{
	float throttle = 0.f;
	float steering = 0.f;
};

void updateRocketPhysics(const RocketParameters& parameters,
						 RocketState& state,
						 const PhysicsInput& input,
						 float deltaTimeSeconds);
