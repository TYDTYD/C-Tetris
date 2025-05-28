#pragma once
#include <vector>
using namespace std;

extern int blockSize;

void CreateBlock();

class Block {
public:
	pair<int, int> origin;
	vector<pair<int, int>> offsets;
	bool collision;

	void Rotate();
	void Move(int dx, int dy);
	void DropBlock();
	bool isContactedBlock(int x = 0, int y = 0);

	Block(pair<int, int> origin, vector<pair<int, int>> offsets) : origin(origin), offsets(offsets), collision(false) {}
};

extern vector<Block> blocks;

