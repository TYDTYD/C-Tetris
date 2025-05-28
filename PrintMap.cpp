#include <iostream>
#include <vector>
#include "Wall.h"
#include "Tetris.h"
using namespace std;
int mapWidth = 12;
int mapHeight = 22;

void PrintMap() {
	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			switch (map[i][j]) {
			case EMPTY:
				cout << "  ";
				break;
			case WALL:
				cout << "бр";
				break;
			case BLOCK:
				cout << "бс";
				break;
			case PREVIEW:
				cout << "в╔";
				break;
			}
		}
		cout << '\n';
	}
}

void InitInputMap(vector<pair<int, int>>& pos) {
	for (const auto& p : pos) {
		if (p.first >= 0 && p.first < mapHeight && p.second >= 0 && p.second < mapWidth) {
			map[p.first][p.second] = EMPTY;
		}
	}
}