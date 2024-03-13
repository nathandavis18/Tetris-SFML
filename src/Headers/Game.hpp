#pragma once
#include <array>

#include "Globals.hpp"
#include "BlockState.hpp"
#include "Blocks.hpp"
#include "Renderer.hpp"

using State = BlockState::State;

class Game{
	sf::Event event{};
public:
	Game();
	void newPiece();
	bool getPiece(u8 x, u8 y, std::unique_ptr<BlockState::Piece>&, u8 rotation);
	
	void loop();
	void getLevel();
	void setTimeNextDrop();
	void updateBoard();
	bool isFullRow(int y);
	void clearLines();

	bool hasCollided();
	bool hasLost();
	
	bool canWallKick(u8 rotation);
	void wallKick();

	bool isValidMove(Move move);
	void input();

	u8 getBottom(bool = 0);
	void renderGame();
	void renderBoard();
	void renderBorder();
	void renderPiece(PieceToDraw piece, std::unique_ptr<BlockState::Piece>&, u8 rotation, u8 yAmount = 0);
	void renderText();
private:
	std::array<u8, game_width*board_height> board;
	u8 level = 0;
	u32 lines = level * 10;
	u8 clearedLines = 0;
	u8 yClearLevel = 0;
	std::array<u8, 30> framesPerDrop = { //From the tetris wiki
		48, 43, 38, 33, 28, 23, 18, 13, 8, 6,
		5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 1
	};
	double timeToNextDrop = framesPerDrop[level] / static_cast<double>(60);


	Renderer renderer;

	std::unique_ptr<State> state;
	Blocks blocks;

	sf::Time time = sf::milliseconds(1000);
	sf::Clock clock;
	sf::Color ghostColor = sf::Color(150, 0, 0, 100);
	sf::Color ghostOutline = sf::Color(50, 50, 50, 50);
};

