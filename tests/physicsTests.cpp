#include "physicsEngine.h"
#include "world.h"
#include <cmath>
#include <cstdlib>
#include <iostream>

void check(bool condition, const char* message) {
    if (!condition) { std::cerr << "FAIL: " << message << '\n'; std::exit(1); }
}
RocketState launch(const RocketParameters& p, float dt, bool expectedSuccess) {
    RocketState s;
    s.fuelMassKg = p.fuelMassKg;
    s.angleDegrees = p.launchAngleDegrees;
    bool coast = false, descent = false;
    for (int i = 0; i < 2000000 && evaluateFlight(p, s) == SimulationResult::Flying; ++i) {
        updateRocketPhysics(p, s, {1.f, 0.f}, dt);
        coast |= s.fuelMassKg == 0.f && s.verticalVelocityMetersPerSecond > 0.f;
        descent |= s.verticalVelocityMetersPerSecond < 0.f;
        check(std::isfinite(s.altitudeMeters) && s.altitudeMeters >= 0.f, "finite, nonnegative altitude");
        check(s.fuelMassKg >= 0.f, "nonnegative fuel");
    }
    check(evaluateFlight(p,s) == (expectedSuccess ? SimulationResult::Success : SimulationResult::Failed), "flight outcome");
    if (!expectedSuccess && s.hasLiftedOff) {
        check(coast && descent && s.groundImpact, "insufficient flight coasts, descends, impacts");
        check(s.altitudeMeters == 0.f && s.verticalVelocityMetersPerSecond == 0.f, "stopped at ground");
    }
    return s;
}
int main() {
    RocketParameters p;
    RocketState s;
    s.altitudeMeters = 1000.f;
    s.hasLiftedOff = true;
    updateRocketPhysics(p,s,{1.f,0.f},1.f);
    check(s.verticalVelocityMetersPerSecond < -9.f, "empty tank cannot produce thrust");
    p.burnRateKgPerSecond = 0.f;
    s = {}; s.altitudeMeters=1000.f; s.fuelMassKg=10.f;
    updateRocketPhysics(p,s,{1.f,0.f},1.f);
    check(s.verticalVelocityMetersPerSecond < 0.f && s.fuelMassKg == 10.f, "zero flow cannot produce thrust");
    p = {}; p.dragCoefficient=0.f;
    s = {}; s.altitudeMeters=1000.f; s.fuelMassKg=0.01f;
    updateRocketPhysics(p,s,{1.f,0.f},1.f/120.f);
    check(s.fuelMassKg == 0.f && s.verticalVelocityMetersPerSecond < 0.f, "partial-step burnout limits impulse");
    p = {}; p.fuelMassKg=100.f;
    for (float dt : {1.f/120.f, 1.f/60.f, 1.f/6.f}) launch(p,dt,false);
    p = {};
    launch(p,1.f/60.f,true);
    p.engineThrustKn=1.f; launch(p,1.f/6.f,false);
    p = {}; p.fuelMassKg=0.f; launch(p,1.f/60.f,false);
    p = {}; p.dryMassKg=0.f; launch(p,1.f/60.f,false);
    p = {}; s = {}; s.altitudeMeters=99999.f;
    check(evaluateFlight(p,s)==SimulationResult::Flying,"below boundary is not success");
    s.altitudeMeters=100000.f;
    check(evaluateFlight(p,s)==SimulationResult::Success,"100 km is success");
    s = {}; s.altitudeMeters=1000.f; s.hasLiftedOff=true;
    s.verticalVelocityMetersPerSecond=-100000.f;
    updateRocketPhysics(p,s,{0.f,0.f},1.f/120.f);
    check(s.verticalVelocityMetersPerSecond <= 0.f && s.altitudeMeters <= 1000.f,"drag cannot catapult descending rocket");
    std::cout << "All physics regression checks passed\n";
}
