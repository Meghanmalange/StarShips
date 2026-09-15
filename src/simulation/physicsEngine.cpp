#include "physicsEngine.h"
#include <algorithm>
#include <cmath>
#include "world.h"

SimulationResult evaluateFlight(const RocketParameters& parameters, const RocketState& state)
{
    if (!(parameters.dryMassKg > 0.f) || !std::isfinite(state.altitudeMeters) ||
        !std::isfinite(state.verticalVelocityMetersPerSecond) ||
        !std::isfinite(state.horizontalVelocityMetersPerSecond))
        return SimulationResult::Failed;
    if (state.altitudeMeters >= Earth::targetAltitudeMeters)
        return SimulationResult::Success;
    if (state.groundImpact || (!state.hasLiftedOff && state.altitudeMeters <= 0.f &&
        (state.fuelMassKg <= 0.f || parameters.burnRateKgPerSecond <= 0.f ||
         parameters.engineThrustKn <= 0.f)))
        return SimulationResult::Failed;
    return SimulationResult::Flying;
}

void updateRocketPhysics(const RocketParameters& parameters, RocketState& state,
                         const PhysicsInput& input, float deltaTimeSeconds)
{
    if (!std::isfinite(deltaTimeSeconds) || deltaTimeSeconds <= 0.f ||
        evaluateFlight(parameters, state) != SimulationResult::Flying)
        return;

    // Bound integration steps independently of rendering and time acceleration.
    double remaining = deltaTimeSeconds;
    while (remaining > 0.0 && evaluateFlight(parameters, state) == SimulationResult::Flying)
    {
        double dt = std::min(remaining, 1.0 / 120.0);
        const double throttle = std::isfinite(input.throttle) ? std::clamp(input.throttle, 0.f, 1.f) : 0.f;
        const double flow = parameters.engineThrustKn > 0.f ?
            std::max(0.f, parameters.burnRateKgPerSecond) * throttle : 0.0;
        const bool burning = state.fuelMassKg > 0.f && flow > 0.0;
        // Split exactly at burnout so no thrust is applied during the coast interval.
        if (burning) dt = std::min(dt, state.fuelMassKg / flow);
        const double burned = burning ? std::min(double(state.fuelMassKg), flow * dt) : 0.0;
        const double mass = parameters.dryMassKg + state.fuelMassKg - burned * 0.5;
        state.fuelMassKg = static_cast<float>(std::max(0.0, state.fuelMassKg - burned));
        const double altitude = std::max(0.f, state.altitudeMeters);
        const double radiusRatio = Earth::radiusMeters / (Earth::radiusMeters + altitude);
        const double gravity = Earth::gravityMetersPerSecondSquared * radiusRatio * radiusRatio;
        const double angle = state.angleDegrees * 3.141592653589793 / 180.0;
        const double thrust = burning ? parameters.engineThrustKn * 1000.0 * throttle : 0.0;
        double vx = state.horizontalVelocityMetersPerSecond + thrust * std::sin(angle) / mass * dt;
        double vy = state.verticalVelocityMetersPerSecond + (thrust * std::cos(angle) / mass - gravity) * dt;
        const double density = Earth::seaLevelDensityKgPerCubicMeter * std::exp(-altitude / Earth::scaleHeightMeters);
        // Implicit quadratic drag opposes motion without overshooting through zero.
        const double drag = 0.5 * density * std::max(0.f, parameters.dragCoefficient) *
                            Earth::rocketReferenceAreaSquareMeters / mass;
        const double damping = 2.0 / (1.0 + std::sqrt(1.0 + 4.0 * drag * dt * std::hypot(vx, vy)));
        vx *= damping;
        vy *= damping;
        if (!state.hasLiftedOff && state.altitudeMeters <= 0.f && vy <= 0.0)
        {
            vx = vy = 0.0; // Launchpad supports the weight while fuel burns.
        }
        state.horizontalVelocityMetersPerSecond = static_cast<float>(vx);
        state.verticalVelocityMetersPerSecond = static_cast<float>(vy);
        state.horizontalPositionMeters += static_cast<float>(vx * dt);
        state.altitudeMeters += static_cast<float>(vy * dt);
        if (state.altitudeMeters > 0.f) state.hasLiftedOff = true;
        if (state.altitudeMeters <= 0.f && state.hasLiftedOff)
        {
            state.altitudeMeters = 0.f;
            state.verticalVelocityMetersPerSecond = 0.f;
            state.horizontalVelocityMetersPerSecond = 0.f;
            state.groundImpact = true;
        }
        state.elapsedSeconds += static_cast<float>(dt);
        remaining -= dt;
    }
}
