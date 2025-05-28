#pragma once
#include "Wall.h"
#include <vector>
enum Type {
	EMPTY,
	WALL,
	BLOCK,
	PREVIEW,
};

extern int mapWidth;
extern int mapHeight;

void PrintMap();

void InitInputMap(vector<pair<int, int>>& pos);