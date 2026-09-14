#include "physicsEngine.h"

#include <algorithm>
#include <cmath>

#include "world.h"

void updateRocketPhysics(const RocketParameters& parameters,
                         RocketState& state,
                         const PhysicsInput& input,
                         float deltaTimeSeconds)
{
    if (deltaTimeSeconds <= 0.f)
    {
        return;
    }

    const float throttle = std::clamp(input.throttle, 0.f, 1.f);
    const float fuelBurned = std::min(state.fuelMassKg,
                                      parameters.burnRateKgPerSecond * throttle * deltaTimeSeconds);
    state.fuelMassKg -= fuelBurned;

    const float massKg = std::max(1.f, parameters.dryMassKg + state.fuelMassKg);
    const float altitude = std::max(0.f, state.altitudeMeters);
    const float density = Earth::seaLevelDensityKgPerCubicMeter *
                          std::exp(-altitude / Earth::scaleHeightMeters);
    const float speed = std::hypot(state.horizontalVelocityMetersPerSecond,
                                   state.verticalVelocityMetersPerSecond);
    const float dragForce = 0.5f * density * speed * speed *
                            parameters.dragCoefficient * Earth::rocketReferenceAreaSquareMeters;
    const float dragX = speed > 0.f ? dragForce * state.horizontalVelocityMetersPerSecond / speed : 0.f;
    const float dragY = speed > 0.f ? dragForce * state.verticalVelocityMetersPerSecond / speed : 0.f;
    const float angleRadians = state.angleDegrees * 3.14159265359f / 180.f;
    const float thrust = parameters.engineThrustKn * 1000.f * throttle;

    const float horizontalAcceleration = (thrust * std::sin(angleRadians) - dragX) / massKg;
    const float verticalAcceleration = (thrust * std::cos(angleRadians) - dragY) / massKg -
                                       Earth::gravityMetersPerSecondSquared;

    state.horizontalVelocityMetersPerSecond += horizontalAcceleration * deltaTimeSeconds;
    state.verticalVelocityMetersPerSecond += verticalAcceleration * deltaTimeSeconds;
    state.horizontalPositionMeters += state.horizontalVelocityMetersPerSecond * deltaTimeSeconds;
    state.altitudeMeters += state.verticalVelocityMetersPerSecond * deltaTimeSeconds;
    state.elapsedSeconds += deltaTimeSeconds;
}