#pragma once

struct RocketParameters
{
	float dryMassKg = 1500.f;
	float fuelMassKg = 3500.f;
	float engineThrustKn = 120.f;
	float burnRateKgPerSecond = 25.f;
	float dragCoefficient = 0.35f;
	float launchAngleDegrees = 90.f;
};

struct RocketState
{
	float altitudeMeters = 0.f;
	float horizontalPositionMeters = 0.f;
	float verticalVelocityMetersPerSecond = 0.f;
	float horizontalVelocityMetersPerSecond = 0.f;
	float fuelMassKg = 0.f;
	float angleDegrees = 90.f;
	float elapsedSeconds = 0.f;
};
