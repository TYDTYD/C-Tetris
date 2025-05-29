# C-Tetris
C++로 구현한 테트리스입니다.


![bandicam2025-05-2823-28-20-181-ezgif com-video-to-gif-converter](https://github.com/user-attachments/assets/659dc09b-57fb-4ce1-b543-03ec17c075fb)


<details>
  <summary>
    Block 종류 및 규칙 정의
  </summary>
  
  ```cpp
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
  ```
</details>


<details>
	<summary>
		입출력 관련 지원 함수
	</summary>
	
```cpp
#include "Helper.h"
#include <Windows.h>

Dir HandleKeyInput()
{
	if (GetAsyncKeyState(VK_LEFT) & 0x01)
	{
		return Dir::Left;
	}
	else if (GetAsyncKeyState(VK_RIGHT) & 0x01)
	{
		return Dir::Right;
	}
	else if (GetAsyncKeyState(VK_UP) & 0x01)
	{
		return Dir::Rotate;
	}
	else if (GetAsyncKeyState(VK_SPACE) & 0x01) {
		return Dir::Space;
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x01) {
		return Dir::Down;
	}
	return Dir::None;
}

// 하나의 포장용? 래핑함수.
void SetCursorPosition(int x, int y)
{
	HANDLE output = ::GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { (SHORT)x, (SHORT)y };
	::SetConsoleCursorPosition(output, pos);
}

void SetCursorOnOff(bool visible)
{
	HANDLE output = ::GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	::GetConsoleCursorInfo(output, &cursorInfo);
	cursorInfo.bVisible = visible;
	::SetConsoleCursorInfo(output, &cursorInfo);
}

void SetCursorColor(Color color, Color bg)
{
	int code = (bg << 4) | color;
	//int code = color;
	// 색상코드: 배경색(4bit) + 글자색(4bit) 조합
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, code);
}
```
</details>

<details>
	<summary>
		Block 위치 표시 함수
	</summary>

```cpp
#include <iostream>
#include "PrintBlock.h"
#include "Block.h"
#include "Tetris.h"
using namespace std;

void PrintBlock(Block block, int rotation) {
	for (const auto& offset : block.offsets)
		map[cursorPos.first + offset.first][cursorPos.second + offset.second] = 2;
}
```
</details>

<details>
	<summary>
		맵 출력 함수
	</summary>
	
```cpp
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
				cout << "¡à";
				break;
			case BLOCK:
				cout << "¡á";
				break;
			case PREVIEW:
				cout << "¢É";
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
```
</details>
