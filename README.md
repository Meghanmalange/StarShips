# StarShips

A C++/SFML rocket flight game.

## Mission rules

- Pass by reaching 100,000 metres (100 km) above the launch surface, assumed to be sea level. This is the conventional Karman line: https://www.fai.org/page/icare-boundary. The atmosphere thins gradually; this is a gameplay boundary, not an altitude where all air disappears. Reaching it does not mean achieving orbit or escaping Earth's gravity.
- An airborne rocket that stays below the target continues through powered flight, coast and descent. Ground contact ends the mission as a failure and stops motion.
- A rocket unable to lift off stays supported on the pad while burning fuel, allowing it to lift off as its mass decreases. If fuel runs out, or its engine has no thrust or fuel flow, the grounded mission fails.
- Fuel exhaustion alone is not failure: momentum may still carry the rocket to the target.

## Physics

Thrust requires fuel and positive fuel flow, with steps split at burnout. Mass includes remaining propellant. Gravity decreases with altitude. Exponential atmospheric density and quadratic drag approximate air resistance. Bounded integration steps and implicit drag prevent numerical velocity reversals. Dry mass must be positive; the configuration UI enforces a 100 kg minimum. Tilt is measured from vertical (0 degrees points up).

This is a simplified ascent model with a flat ground plane, fixed reference area and drag coefficient, and independently configurable thrust and fuel flow. It does not model orbital motion, staging, heating or structural failure.

## Build and verify

With CMake, a C++17 compiler and SFML 3 installed:

```
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Regression tests cover unsuccessful and successful flights, coast/descent/impact, launchpad support, invalid dry mass, zero fuel flow, partial-step burnout, multiple update intervals and high-speed drag stability.
