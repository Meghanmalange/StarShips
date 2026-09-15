#pragma once

struct RocketParameters
{
	float dryMassKg = 1500.f;
	float fuelMassKg = 3500.f;
	float engineThrustKn = 120.f;
	float burnRateKgPerSecond = 25.f;
	float dragCoefficient = 0.35f;
	float launchAngleDegrees = 0.f; // Tilt from vertical.
};

struct RocketState
{
	float altitudeMeters = 0.f;
	float horizontalPositionMeters = 0.f;
	float verticalVelocityMetersPerSecond = 0.f;
	float horizontalVelocityMetersPerSecond = 0.f;
	float fuelMassKg = 0.f;
	float angleDegrees = 0.f;
	float elapsedSeconds = 0.f;
	bool hasLiftedOff = false;
	bool groundImpact = false;
};
