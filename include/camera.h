#pragma once

#include <SFML/Graphics/View.hpp>

class Camera
{
public:
	Camera();
	void follow(float horizontalPositionMeters, float altitudeMeters);
	const sf::View& view() const;

private:
	sf::View cameraView;
};
