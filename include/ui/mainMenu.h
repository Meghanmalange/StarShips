#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class MainMenu
{
public:
	MainMenu();

	void draw(sf::RenderWindow& window) const;
	bool startClicked(sf::Vector2i position) const;

private:
	Font font;
	Text title;
	Text startLabel;
	Text musicLabel;
	Text soundLabel;
	bool fontLoaded;
};
