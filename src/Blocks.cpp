#include <chrono>

#include "Headers/Blocks.hpp"

Blocks::Blocks() {
	gen.seed(std::chrono::system_clock::now().time_since_epoch().count());
}

const BlockState::Piece& Blocks::getBlock(){
	std::uniform_int_distribution<int> rand(0, blocks.size() - 1);
	return blocks[rand(gen)]; //Returns a block from the list of blocks
}