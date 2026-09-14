#include "ui/mainMenu.h"
using namespace sf;

MainMenu::MainMenu()
	: title(font, "StarShips", 64),
	  startLabel(font, "START GAME", 28),
	  musicLabel(font, "MUSIC", 20),
	  soundLabel(font, "SOUND", 20),
	  fontLoaded(false)
{
	fontLoaded = font.openFromFile("C:/Windows/Fonts/seguisb.ttf");
	if (!fontLoaded)
	{
		fontLoaded = font.openFromFile("C:/Windows/Fonts/arial.ttf");
	}

	title.setPosition({265.f, 130.f});
	startLabel.setPosition({306.f, 322.f});
	musicLabel.setPosition({296.f, 438.f});
	soundLabel.setPosition({451.f, 438.f});
}

void MainMenu::draw(RenderWindow& window) const
{
	RectangleShape startButton({330.f, 72.f});
	startButton.setPosition({235.f, 300.f});
	startButton.setFillColor(Color(52, 112, 166));

	RectangleShape musicButton({145.f, 58.f});
	musicButton.setPosition({235.f, 425.f});
	musicButton.setFillColor(Color(37, 67, 92));

	RectangleShape soundButton({145.f, 58.f});
	soundButton.setPosition({420.f, 425.f});
	soundButton.setFillColor(Color(37, 67, 92));

	window.draw(startButton);
	window.draw(musicButton);
	window.draw(soundButton);

	if (fontLoaded)
	{
		window.draw(title);
		window.draw(startLabel);
		window.draw(musicLabel);
		window.draw(soundLabel);
	}
}

bool MainMenu::startClicked(Vector2i position) const
{
	return sf::FloatRect({235.f, 300.f}, {330.f, 72.f}).contains(
		{static_cast<float>(position.x), static_cast<float>(position.y)});
}
