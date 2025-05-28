#include <iostream>
#include "PrintBlock.h"
#include "Block.h"
#include "Tetris.h"
using namespace std;

void PrintBlock(Block block, int rotation) {
	for (const auto& offset : block.offsets)
		map[cursorPos.first + offset.first][cursorPos.second + offset.second] = 2;
}