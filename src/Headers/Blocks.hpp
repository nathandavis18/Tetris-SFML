#pragma once
#include <vector>
#include <random>

#include "Globals.hpp"
#include "BlockState.hpp"

class Blocks {
public:
	Blocks();
	const BlockState::Piece& getBlock();
private:
	const std::vector<u8> oBlock = {
		1, 1,
		1, 1
	};
	const std::vector<u8> sBlock = {
		0, 1, 1,
		1, 1, 0,
		0, 0, 0
	};
	const std::vector<u8> zBlock = {
		1, 1, 0,
		0, 1, 1,
		0, 0, 0
	};
	const std::vector<u8> lBlock = {
		0, 1, 0,
		0, 1, 0,
		0, 1, 1
	};
	const std::vector<u8> jBlock = {
		0, 1, 0,
		0, 1, 0,
		1, 1, 0
	};
	const std::vector<u8> tBlock = {
		0, 1, 0,
		1, 1, 1,
		0, 0, 0
	};
	const std::vector<u8> iBlock = {
		0, 0, 1, 0,
		0, 0, 1, 0,
		0, 0, 1, 0,
		0, 0, 1, 0
	};
	const std::vector<BlockState::Piece> blocks = {
		BlockState::Piece(oBlock, 2),
		BlockState::Piece(sBlock, 3),
		BlockState::Piece(zBlock, 3),
		BlockState::Piece(lBlock, 3),
		BlockState::Piece(jBlock, 3),
		BlockState::Piece(tBlock, 3),
		BlockState::Piece(iBlock, 4)
	};

	std::default_random_engine gen;
};