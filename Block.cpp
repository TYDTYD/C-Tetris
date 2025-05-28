#include <vector>
#include "Block.h"
#include "Tetris.h"
using namespace std;

int blockSize = 7;
vector<Block> blocks;

// x,y => y,-x

void CreateBlock() {
	int blockSize = 7; // 블록 종류 개수

	vector<pair<int, int>> offset;
	offset.push_back({ 0,0 });
	offset.push_back({ 1,0 });
	offset.push_back({ 2,0 });
	offset.push_back({ 3,0 });
	// 블록 종류 설정
	Block block1 = Block(cursorPos, offset);
	offset.clear();
	offset.push_back({ 0,0 });
	offset.push_back({ 1,0 });
	offset.push_back({ 2,0 });
	offset.push_back({ 2,1 });
	Block block2 = Block(cursorPos, offset);
	offset.clear();
	offset.push_back({ 0,0 });
	offset.push_back({ 1,0 });
	offset.push_back({ 2,0 });
	offset.push_back({ 1,-1 });
	Block block3 = Block(cursorPos, offset);
	offset.clear();
	offset.push_back({ 0,0 });
	offset.push_back({ 0,-1 });
	offset.push_back({ 1,0 });
	offset.push_back({ 1,-1 });
	Block block4 = Block(cursorPos, offset);
	offset.clear();
	offset.push_back({ 0,0 });
	offset.push_back({ 1,0 });
	offset.push_back({ 2,0 });
	offset.push_back({ 2,-1 });
	Block block5 = Block(cursorPos, offset);
	offset.clear();
	offset.push_back({ 0,0 });
	offset.push_back({ 1,0 });
	offset.push_back({ 1,-1 });
	offset.push_back({ 0,1 });
	Block block6 = Block(cursorPos, offset);
	offset.clear();
	offset.push_back({ 0,0 });
	offset.push_back({ 0,-1 });
	offset.push_back({ 1,0 });
	offset.push_back({ 1,1 });
	Block block7 = Block(cursorPos, offset);
	// 커서 위치 = 블록 위치
	blocks.push_back(block1);
	blocks.push_back(block2);
	blocks.push_back(block3);
	blocks.push_back(block4);
	blocks.push_back(block5);
	blocks.push_back(block6);
	blocks.push_back(block7);
}

void Block::DropBlock() {
	bool plag = true;
	while (plag) {
		for (const auto& offset : offsets) {
			if (map[cursorPos.first + offset.first][cursorPos.second + offset.second] == WALL || map[cursorPos.first + offset.first][cursorPos.second + offset.second] == BLOCK) {
				cursorPos.first--;
				return;
			}
		}
		cursorPos.first++;
	}
	return;
}

void Block::Rotate() {
	int mn = INT16_MAX;
	for (auto& offset : offsets) {
		mn = min(offset.second, mn);
	}

	if (mn < 0) {
		for (auto& offset : offsets) {
			offset.second -= mn;
		}
	}

	for (auto& offset : offsets) {
		swap(offset.first, offset.second);
		offset.second = -offset.second;
	}
}

void Block::Move(int dx, int dy) {
	origin.first += dx;
	origin.second += dy;
}

bool Block::isContactedBlock(int x, int y) {
	for (auto& offset : offsets) {
		if (map[cursorPos.first + x + offset.first][cursorPos.second + y + offset.second] == BLOCK) {
			return true;
		}
	}
	return false;
}