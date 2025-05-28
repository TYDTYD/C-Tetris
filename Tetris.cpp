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