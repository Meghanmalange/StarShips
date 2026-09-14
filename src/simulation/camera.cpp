#include "camera.h"

#include "world.h"

Camera::Camera()
    : cameraView(sf::Vector2f(0.f, 0.f), sf::Vector2f(800.f, 800.f))
{
}

void Camera::follow(float horizontalPositionMeters, float altitudeMeters)
{
    cameraView.setCenter({horizontalPositionMeters * Rendering::pixelsPerMeter,
                          -altitudeMeters * Rendering::pixelsPerMeter});
}

const sf::View& Camera::view() const
{
    return cameraView;
}