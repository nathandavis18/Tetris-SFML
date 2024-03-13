#include <string>

#include "Headers/Game.hpp"


Game::Game() {
	state = std::make_unique<BlockState::State>(BlockState::State());
	state->piece = std::make_unique<BlockState::Piece>(blocks.getBlock());
	state->nextPiece = std::make_unique<BlockState::Piece>(blocks.getBlock());
	std::fill(board.begin(), board.end(), 0);
	time = sf::milliseconds(timeToNextDrop * 1000);
	getLevel();
	loop();
}
void Game::newPiece() {
	state->piece = std::move(state->nextPiece); //Next piece becomes current piece
	state->nextPiece = std::make_unique<BlockState::Piece>(blocks.getBlock()); //Next piece becomes a new piece

	state->xOffset = game_width / 2 - 2;
	state->yOffset = 0;
	state->rotation = 0;
}
bool Game::getPiece(u8 x, u8 y, std::unique_ptr<BlockState::Piece>& p, u8 rotation) {
	switch (rotation) {
	case 0:
		return (p->data[y * p->width + x]);
	case 1:
		return (p->data[(p->width - x - 1) * p->width + y]);
	case 2:
		return (p->data[(p->width - y - 1) * p->width + (p->width - x - 1)]);
	case 3:
		return (p->data[(p->width * x) + (p->width - y - 1)]);
	default:
		return 0;
	}
}

void Game::loop() {
	while (renderer.getWindow().isOpen() && !quit) {
		input();
		renderGame();
		if (time.asMilliseconds() <= 0) {
			++state->yOffset;
			setTimeNextDrop();
		}
		if (hasCollided()) {
			--state->yOffset;
			updateBoard();
			clearLines();
			quit = hasLost();
			getLevel();
			newPiece();
		}

		time -= clock.restart();
	}
	while (quit && renderer.getWindow().isOpen()) {
		renderer.clearRenderer();
		input();
		renderBoard();
		renderBorder();
		renderText();
		renderer.showRenderer();
	}
}
void Game::getLevel() {
	level = lines / 10;
	if (level > 29) {
		level = 29; //Level 29 (30 if not 0 indexing) is the max level
	}
}
void Game::setTimeNextDrop() {
	timeToNextDrop = framesPerDrop[level] / static_cast<double>(60);
	time = sf::milliseconds(timeToNextDrop * 1000);
}
void Game::updateBoard() { //Board gets updated when the playing piece collides with the board
	for (int x = 0; x < state->piece->width; ++x) {
		for (int y = 0; y < state->piece->width; ++y) {
			if (getPiece(x, y, state->piece, state->rotation)) {
				s8 bigX = state->xOffset + x;
				s8 bigY = state->yOffset + y;
				if (bigY >= 0)
					board[bigY * game_width + bigX] = 1;
				else {
					quit = true;
				}
			}
		}
	}
}
bool Game::isFullRow(int y) {
	for (int x = 0; x < game_width; ++x) { //Every tile in a given row
		if (!board[y * game_width + x])//If a single spot in a row isn't a block we don't need to keep checking it
			return false;
	}
	return true;
}
void Game::clearLines() {
	clearedLines = 0;
	for (int y = 0; y < game_height; ++y) { //Every row
		if (isFullRow(y)) {
			for (int x = 0; x < game_width; ++x) {
				board[y * game_width + x] = 0; //If its a full row, we clear it
			}
			++clearedLines;
			yClearLevel = y;
		}
	}
	lines += clearedLines; //Updating total amount of lines cleared
	while (clearedLines > 0) { //If we clear 1 line, we drop the rest of the board one time. Two times for 2 cleared lines, and so on
		for (int y = yClearLevel; y > 0; --y) {
			for (int x = 0; x < game_width; ++x) {
				board[y * game_width + x] = board[(y - 1) * game_width + x];
			}
		}
		--clearedLines;
	}
}

bool Game::hasCollided() {
	for (int x = 0; x < state->piece->width; ++x) {
		for (int y = 0; y < state->piece->width; ++y) {
			if (getPiece(x, y, state->piece, state->rotation)) {
				s8 bigX = state->xOffset + x;
				s8 bigY = state->yOffset + y;
				if (bigY >= 0 && (board[bigY * game_width + bigX] == 1 || bigY >= game_height)) {
					return true;
				}
			}
		}
	}
	return false;
}
bool Game::hasLost() {
	for (int x = 0; x < game_width; ++x) {
		if (board[x]) //If there is a piece on the top row, then we lose
			return true;
	}
	return false;
}

bool Game::canWallKick(u8 rotation) {
	for (int x = 0; x < state->piece->width; ++x) {
		for (int y = 0; y < state->piece->width; ++y) {
			if (getPiece(x, y, state->piece, rotation)) {
				s8 bigX = state->xOffset + x;
				s8 bigY = state->yOffset + y;
				if (board[bigY * game_width + bigX]) {
					return false;
				}
			}
		}
	}
	return true;
}
void Game::wallKick() {
	for (int x = 0; x < state->piece->width; ++x) {
		for (int y = 0; y < state->piece->width; ++y) {
			if (getPiece(x, y, state->piece, state->rotation)) {
				s8 bigX = state->xOffset + x;
				s8 bigY = state->yOffset + y;
				if (bigX < 0) {
					state->xOffset -= bigX;
				}
				if (bigX >= game_width) {
					state->xOffset -= (bigX - game_width) + 1;
				}
				if (bigY >= game_height) {
					--state->yOffset;
				}
			}
		}
	}
}

u8 Game::getBottom(bool ghost) {
	u8 checkAmount;
	u8 finalAmount = game_height;
	for (int x = 0; x < state->piece->width; ++x) {
		for (int y = 0; y < state->piece->width; ++y) {
			if (getPiece(x, y, state->piece, state->rotation)) {
				checkAmount = 0;
				s8 bigX = state->xOffset + x;
				s8 bigY = state->yOffset + y + ghost; //If we are getting the ghost piece, it is 1 value higher
				while (board[bigY * game_width + bigX] != 1 && bigY < game_height) {
					++checkAmount;
					++bigY;
				}
				if (checkAmount < finalAmount) {
					finalAmount = checkAmount;
				}
			}
		}
	}
	return finalAmount;
}
//Sends everything to the renderer to be drawn
void Game::renderGame() {
	renderer.clearRenderer();

	renderPiece(PieceToDraw::NormalPiece, state->piece, state->rotation);
	u8 ghostPieceDrop = getBottom(1);
	renderPiece(PieceToDraw::GhostPiece, state->piece, state->rotation, ghostPieceDrop);
	renderPiece(PieceToDraw::NextPiece, state->nextPiece, 0);

	renderBoard();
	renderBorder();
	renderText();

	renderer.showRenderer();
}
//Draws the piece you are playing, the ghost piece for hard-drop accuracy, and the upcoming piece
void Game::renderPiece(PieceToDraw piece, std::unique_ptr<BlockState::Piece>& p, u8 rotation, u8 yAmount) {
	for (int y = 0; y < p->width; ++y) {
		for (int x = 0; x < p->width; ++x) {
			if (getPiece(x, y, p, rotation)) {
				switch (piece) {
				case PieceToDraw::NormalPiece:
					renderer.draw(x + state->xOffset + boardXOffset, y + state->yOffset + boardYOffset, sf::Color::Red, sf::Color::White);
					break;
				case PieceToDraw::GhostPiece:
					renderer.draw(x + state->xOffset + boardXOffset, y + state->yOffset + boardYOffset + yAmount, ghostColor, ghostOutline);
					break;
				case PieceToDraw::NextPiece:
					renderer.draw(x + game_width + 14 - p->width, y + 3, sf::Color::Blue, sf::Color::White);
					break;
				}
			}
		}
	}
}
void Game::renderBoard() { //Draws all pieces that are currently on the board
	for (int x = 0; x < game_width; ++x) {
		for (int y = 0; y < game_height; ++y) {
			if (board[y * game_width + x] == 1)
				renderer.draw(x + boardXOffset, y + boardYOffset, sf::Color::Green, sf::Color::White);
		}
	}
}
void Game::renderBorder() { //Draws the border around the playable area
	for (int x = 7; x < game_width + 9; ++x) {
		renderer.draw(x, 0, sf::Color::White, sf::Color::Blue);
		renderer.draw(x, game_height + 1, sf::Color::White, sf::Color::Blue);
	}
	for (int y = 0; y < game_height + 1; ++y) {
		renderer.draw(7, y, sf::Color::White, sf::Color::Blue);
		renderer.draw(game_width + 8, y, sf::Color::White, sf::Color::Blue);
	}
}
void Game::renderText() {
	std::string lvlStr = "Level: " + std::to_string(level + 1); //Levels are 1-30 but arrays are 0-indexed, so we add 1 purely for display
	std::string linesStr = "Lines: " + std::to_string(lines);
	renderer.drawText(width - 200, height / 2, lvlStr);
	renderer.drawText(width - 200, height / 2 + 50, linesStr);
}

bool Game::isValidMove(Move move) { //Can't move into the board or another piece
	for (int x = 0; x < state->piece->width; ++x) {
		for (int y = 0; y < state->piece->width; ++y) {
			if (getPiece(x, y, state->piece, state->rotation)) {
				s8 bigX = state->xOffset + x;
				s8 bigY = state->yOffset + y;
				if ((bigX <= 0 || board[(bigY * game_width + bigX - 1)]) && move == Move::Left)
					return false;
				if ((bigX >= game_width - 1 || board[(bigY * game_width + bigX + 1)]) && move == Move::Right)
					return false;
				if (bigY >= game_height && move == Move::Down)
					return false;
			}
		}
	}
	return true;
}
void Game::input() {
	while (renderer.getWindow().pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			renderer.getWindow().close();
			break;
		case sf::Event::EventType::KeyPressed:
			if (event.key.code == sf::Keyboard::Right) {
				if(isValidMove(Move::Right))
					++state->xOffset;
				break;
			}
			if (event.key.code == sf::Keyboard::Left) {
				if(isValidMove(Move::Left))
					--state->xOffset;
				break;
			}
			if (event.key.code == sf::Keyboard::Down) {
				if (isValidMove(Move::Down))
					time = sf::milliseconds(0);
				break;
			}
			if (event.key.code == sf::Keyboard::Up) {
				if (canWallKick((state->rotation + 1) % 4)) {
					state->rotation = (state->rotation + 1) % 4;
						wallKick();
				}
				break;
			}
			if (event.key.code == sf::Keyboard::Z) {
				if (canWallKick((state->rotation + 3) % 4)) {
					state->rotation = (state->rotation + 3) % 4;
					wallKick();
				}
				break;
			}
			if (event.key.code == sf::Keyboard::Space) {
				state->yOffset += getBottom();
				break;
			}
		}
	}
}