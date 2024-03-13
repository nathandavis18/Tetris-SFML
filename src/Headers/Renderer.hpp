#pragma once
#include <string>
#include "../include/SFML/Graphics.hpp"

class Renderer
{
public:
	const Renderer();
	void clearRenderer();
	void showRenderer();
	void draw(const int x, const int y, const sf::Color fill, const sf::Color outline);
	void drawText(const int x, const int y, const std::string strToDisplay);
	sf::RenderWindow& getWindow();
private:
	sf::Font font;
	sf::Text text;
	sf::RenderWindow window;
};

