#include "Wall.h"
#include "Tetris.h"
#include "Block.h"

int wallLeft, wallRight, wallBottom;

void SetWall(int left, int right, int bottom) {
	wallLeft = left;
	wallRight = right;
	wallBottom = bottom;

	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			if (i >= wallBottom || j < wallLeft || j >= wallRight) {
				map[i][j] = 1;
				continue;
			}
			map[i][j] = 0;
		}
	}
}

bool isBlockContactWall(int x, int y, Block block) {
	for (const auto& offset : block.offsets) {
		if (isWall(x + offset.first, y + offset.second))
			return true;
	}
	return false;
}

bool isWall(int x, int y) {
	return (x < 0 || x >= wallBottom || y < wallLeft || y >= wallRight);
}