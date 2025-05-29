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
		맵 출력 함수 구현
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

<details>
	<summary>
		벽 설정 관련 함수 구현
	</summary>

```cpp
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
``` 
</details>

<details>
	<summary>
		테트리스 메인 함수(블록 삭제 규칙 정의)
	</summary>

```cpp
#include <iostream>
#include "Helper.h"
#include "Wall.h"
#include "PrintMap.h"
#include "Block.h"
#include "PrintBlock.h"
#include "Tetris.h"
#include <thread>
#include <chrono>
#include <time.h>
using namespace std;

// 커서 위치 설정
pair<int, int> cursorPos = { 0, mapWidth / 2 }; // 초기 커서 위치 설정
vector<vector<int>> map(mapHeight, vector<int>(mapWidth, 0)); // 맵 초기화
int dx[4] = { 0, 1, 0, -1 };
int dy[4] = { -1, 0, 1, 0 };

void DeleteLine() {
    // 아래쪽부터 검사
    // 삭제되면 삭제된 지점부터 위쪽으로 맵을 아래쪽으로 한번 땡기고 다시 함수 실행
    for (int i = 4; i < wallBottom; i++) {
        int count = 0;
        for (int j = wallLeft; j < wallRight; j++) {
            if (map[i][j] == BLOCK) {
                count++;
            }
        }
        if (count == mapWidth - 2) {
            for (int x = i; x > 4; x--) {
                for (int y = wallLeft; y < wallRight; y++) {
                    map[x][y] = map[x - 1][y];
                }
            }
            return DeleteLine();
        }
    }
    return;
}

void OutputPreviewBlock(Block& block, vector<pair<int, int>>& pos) {
    int mx = INT32_MIN;
    for (const auto& offset : block.offsets) {
        int x = cursorPos.first + offset.first;
        mx = max(x, mx);
    }

    for (int i = 0; i < mapHeight; i++) {
        if (isBlockContactWall(cursorPos.first + i, cursorPos.second, block) || block.isContactedBlock(i, 0)) {
            for (const auto& offset : block.offsets) {
                int x = cursorPos.first + i - 1 + offset.first;
                int y = cursorPos.second + offset.second;
                map[x][y] = PREVIEW;
                pos.push_back({ x,y });
            }
            break;
        }
    }
}

void DeleteBeforePreviewPos(vector<pair<int, int>>& pos) {
    for (const auto& p : pos) {
        if (p.first >= 0 && p.first < mapHeight && p.second >= 0 && p.second < mapWidth) {
            if (map[p.first][p.second] != BLOCK)
                map[p.first][p.second] = EMPTY;
        }
    }
}

void DeleteBeforePos(vector<pair<int, int>>& beforePos, Block& block) {
    beforePos.clear(); // 이전 블록 위치 초기화

    // 블록 위치 저장
    for (const auto& offset : block.offsets) {
        beforePos.push_back({ offset.first + cursorPos.first, offset.second + cursorPos.second }); // 블록 위치 조정
    }
    // 입력 범위에 대해서 초기화 수행
    InitInputMap(beforePos);
}

int main()
{
    srand(unsigned int(time(NULL)));
    // 벽 경계 설정
    SetWall(1, mapWidth - 1, mapHeight - 1);

    CreateBlock();

    SetCursorOnOff(false); // 커서 깜빡거림 금지

    Block block = blocks[rand() % blockSize]; // 랜덤 블록 생성

    time_t before, next;
    double duration;

    before = time(NULL);

    bool isNext = false;

    vector<pair<int, int>> beforePos;
    vector<pair<int, int>> beforePreviewPos;

    using Clock = std::chrono::steady_clock;
    auto lastDropTime = Clock::now();
    const std::chrono::milliseconds dropInterval(500);

    bool isGameOver = false;

    while (true) {
        // 출력 위치 초기화
        SetCursorPosition(0, 0);

        for (int i = 0; i < 4; i++) {
            int count = 0;
            for (int j = 1; j < mapWidth - 1; j++) {
                if (map[i][j] == PREVIEW) {
                    isGameOver = true;
                    break;
                }
            }
            if (isGameOver)
                break;
        }

        if (isGameOver) {
            system("cls");
            SetCursorOnOff(true);
            const string gameOverMsg = "====== GAME OVER ======";
            int centerX = (mapWidth * 2 - gameOverMsg.size()) / 2; // 가로 중앙 계산

            for (int i = 0; i < 3; i++)
                cout << '\n';
            cout << string(centerX, ' ') << gameOverMsg;
            for (int i = 0; i < 8; i++)
                cout << '\n';
            cout << string(centerX, ' ') << "Press Enter to exit..." << endl;

            cin.get(); // 사용자 입력 대기
            break;
        }

        // 다음 블록 생성
        if (isNext) {
            block = blocks[rand() % blockSize];
            cursorPos = { 0,mapWidth / 2 };
            isNext = false;
        }
        else {
            DeleteBeforePos(beforePos, block);
            // 이전 미리보기 블록 위치 제거
            DeleteBeforePreviewPos(beforePreviewPos);
        }

        auto now = Clock::now();
        if (now - lastDropTime >= dropInterval) {
            if (!isBlockContactWall(cursorPos.first + 1, cursorPos.second, block) && !block.isContactedBlock(1)) {
                cursorPos.first += 1; // 블록을 아래로 내림
                DeleteBeforePos(beforePos, block); // 이전 블록 위치 저장
            }
            else
                isNext = true; // 블록이 더 이상 내려갈 수 없으면 다음 블록 생성

            lastDropTime = now; // 마지막 드롭 시간 업데이트
        }

        // 키 입력
        Dir dir = HandleKeyInput();

        switch (dir) {
        case Left:
            if (!isBlockContactWall(cursorPos.first, cursorPos.second - 1, block))
                cursorPos.second -= 1;
            break;
        case Right:
            if (!isBlockContactWall(cursorPos.first, cursorPos.second + 1, block))
                cursorPos.second += 1;
            break;
        case Space:
            block.DropBlock();
            isNext = true;
            break;
        case Rotate: {
            Block temp = block;
            block.Rotate();
            if (isBlockContactWall(cursorPos.first, cursorPos.second, block) || block.isContactedBlock())
                block = temp;
            break;
        }
        case Down:
            if (!isBlockContactWall(cursorPos.first + 1, cursorPos.second, block))
                cursorPos.first += 1;
            break;
        case None:
            break;
        }

        // todo 블록이 어디 떨어질 지 미리보기 출력
        OutputPreviewBlock(block, beforePreviewPos);

        DeleteLine();

        // 블록 출력 좌표 설정
        PrintBlock(block, 0);
        // 맵 출력
        PrintMap();

        // 매 초마다 떨어져야 함
        this_thread::sleep_for(chrono::milliseconds(16)); // 60 프레임
    }
    return 0;
}
```
</details>
