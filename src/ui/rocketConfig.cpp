#include "ui/rocketConfig.h"

#include <algorithm>
#include <array>
#include <string>

using namespace sf;
using namespace std;

namespace{
const float leftMargin = 50.f;
const float firstRow = 155.f;
const float rowSpacing = 65.f;

Text makeText(const Font& font, const string& text, unsigned int size, Vector2f position){
	Text result(font, text, size);
	result.setPosition(position);
	return result;
}

void drawButton(RenderWindow& window, const Font& font, const string& label, Vector2f position, Vector2f size, unsigned int textSize){
	RectangleShape button(size);
	button.setPosition(position);
	button.setFillColor(Color(37, 67, 92));

	Text text(font, label, textSize);
	const FloatRect bounds = text.getLocalBounds();
	text.setPosition({
		position.x + (size.x - bounds.size.x) / 2.f - bounds.position.x,
		position.y + (size.y - bounds.size.y) / 2.f - bounds.position.y});

	window.draw(button);
	window.draw(text);
}

bool contains(Vector2i position, Vector2f topLeft, Vector2f size){
	return FloatRect(topLeft, size).contains({
		static_cast<float>(position.x), static_cast<float>(position.y)});
	}
}

RocketConfig::RocketConfig()
	: fontLoaded(false),
	  dryMass(1500),
	  fuelMass(3500),
	  engineThrust(120),
	  burnRate(25),
	  dragCoefficient(35),
	  launchAngle(90)
{
	fontLoaded = font.openFromFile("C:/Windows/Fonts/seguisb.ttf");
	if (!fontLoaded)
	{
		fontLoaded = font.openFromFile("C:/Windows/Fonts/arial.ttf");
	}
}

void RocketConfig::draw(RenderWindow& window) const{
	window.clear(Color(12, 23, 34));

	if (!fontLoaded){
		return;
	}

	window.draw(makeText(font, "STARSHIPS - rocket configuration", 34, {leftMargin, 55.f}));

	const array<string, 6> labels = {
		"Dry Mass", "Fuel Mass", "Engine Thrust", "Burn Rate", "Drag Coeff.", "Launch Angle"};
	const array<string, 6> values = {
		to_string(dryMass) + " kg",
		to_string(fuelMass) + " kg",
		to_string(engineThrust) + " kN",
		to_string(burnRate) + " kg/s",
		string("0.") + (dragCoefficient < 10 ? "0" : "") + to_string(dragCoefficient),
		to_string(launchAngle) + " deg"};

	for (size_t index = 0; index < labels.size(); ++index){
		const float y = firstRow + rowSpacing * static_cast<float>(index);
		window.draw(makeText(font, labels[index], 23, {leftMargin, y}));
		drawButton(window, font, "-", {330.f, y}, {38.f, 34.f}, 20);
		window.draw(makeText(font, values[index], 23, {385.f, y}));
		drawButton(window, font, "+", {570.f, y}, {38.f, 34.f}, 20);
	}

	window.draw(makeText(font, "Total Mass: " + to_string(dryMass + fuelMass) + " kg",
					 26, {leftMargin, 555.f}));
	drawButton(window, font, "BACK", {leftMargin, 660.f}, {145.f, 42.f}, 21);
	drawButton(window, font, "START", {260.f, 660.f}, {145.f, 42.f}, 21);
}

ConfigAction RocketConfig::handleClick(Vector2i position)
{
	if (contains(position, {leftMargin, 660.f}, {145.f, 42.f}))
	{
		return ConfigAction::Back;
	}

	if (contains(position, {260.f, 660.f}, {145.f, 42.f})){
		return ConfigAction::Start;
	}

	for (int index = 0; index < 6; ++index)
	{
		const float y = firstRow + rowSpacing * static_cast<float>(index);
		if (contains(position, {330.f, y}, {38.f, 34.f}))
		{
			switch (index)
			{
			case 0: dryMass = max(0, dryMass - 100); break;
			case 1: fuelMass = max(0, fuelMass - 100); break;
			case 2: engineThrust = max(0, engineThrust - 5); break;
			case 3: burnRate = max(0, burnRate - 1); break;
			case 4: dragCoefficient = max(0, dragCoefficient - 1); break;
			case 5: launchAngle = max(-30, launchAngle - 5); break;
			}
		}
		else if (contains(position, {570.f, y}, {38.f, 34.f}))
		{
			switch (index)
			{
			case 0: dryMass += 100; break;
			case 1: fuelMass += 100; break;
			case 2: engineThrust += 5; break;
			case 3: burnRate += 1; break;
			case 4: dragCoefficient = min(99, dragCoefficient + 1); break;
			case 5: launchAngle = min(30, launchAngle + 5); break;
			}
		}
	}

	return ConfigAction::None;
}

RocketParameters RocketConfig::parameters() const
{
	return {
		static_cast<float>(dryMass),
		static_cast<float>(fuelMass),
		static_cast<float>(engineThrust),
		static_cast<float>(burnRate),
		static_cast<float>(dragCoefficient) / 100.f,
		static_cast<float>(launchAngle)};
}
