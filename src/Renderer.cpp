#include <filesystem>
#include <iostream>
#include "Headers/Globals.hpp"
#include "Headers/Renderer.hpp"

Renderer::Renderer() : window(sf::VideoMode(width, height), "TETRIS") { 
	std::filesystem::path cwd = std::filesystem::current_path();
	std::cout << cwd << std::endl;
}

void Renderer::clearRenderer() {
	window.clear();
}

void Renderer::showRenderer() {
	window.display();
}

void Renderer::draw(const int x, const int y, const sf::Color fill, const sf::Color outline) {
	sf::RectangleShape rect;

	rect.setFillColor(fill);
	rect.setOutlineColor(outline);
	rect.setOutlineThickness(1);
	
	rect.setSize(sf::Vector2f(size, size));
	rect.setPosition(x * size, y * size);
	
	window.draw(rect);
}

void Renderer::drawText(const int x, const int y, const std::string strToDisplay) {
	if (!font.loadFromFile("Font/tetris-font.ttf")) {
		std::cout << "Error Loading Font" << std::endl;
	}
	else {
		text.setFont(font);
		text.setString(strToDisplay);
		text.setCharacterSize(24);
		text.setFillColor(sf::Color::Cyan);
		text.setStyle(sf::Text::Bold);
		text.setPosition(x, y);
		
		window.draw(text);
	}
}

sf::RenderWindow& Renderer::getWindow() {
	return window;
}