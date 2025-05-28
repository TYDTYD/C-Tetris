#pragma once
#include "Block.h"
extern int wallLeft;
extern int wallRight;
extern int wallBottom;

void SetWall(int left, int right, int bottom);
bool isWall(int x, int y);
bool isBlockContactWall(int x, int y, Block block);