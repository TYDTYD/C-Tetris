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

// Ŀ�� ��ġ ����
pair<int, int> cursorPos = { 0, mapWidth / 2 }; // �ʱ� Ŀ�� ��ġ ����
vector<vector<int>> map(mapHeight, vector<int>(mapWidth, 0)); // �� �ʱ�ȭ
int dx[4] = { 0, 1, 0, -1 };
int dy[4] = { -1, 0, 1, 0 };

void DeleteLine() {
    // �Ʒ��ʺ��� �˻�
    // �����Ǹ� ������ �������� �������� ���� �Ʒ������� �ѹ� ����� �ٽ� �Լ� ����
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
    beforePos.clear(); // ���� ��� ��ġ �ʱ�ȭ

    // ��� ��ġ ����
    for (const auto& offset : block.offsets) {
        beforePos.push_back({ offset.first + cursorPos.first, offset.second + cursorPos.second }); // ��� ��ġ ����
    }
    // �Է� ������ ���ؼ� �ʱ�ȭ ����
    InitInputMap(beforePos);
}

int main()
{
    srand(unsigned int(time(NULL)));
    // �� ��� ����
    SetWall(1, mapWidth - 1, mapHeight - 1);

    CreateBlock();

    SetCursorOnOff(false); // Ŀ�� �����Ÿ� ����

    Block block = blocks[rand() % blockSize]; // ���� ��� ����

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
        // ��� ��ġ �ʱ�ȭ
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
            int centerX = (mapWidth * 2 - gameOverMsg.size()) / 2; // ���� �߾� ���

            for (int i = 0; i < 3; i++)
                cout << '\n';
            cout << string(centerX, ' ') << gameOverMsg;
            for (int i = 0; i < 8; i++)
                cout << '\n';
            cout << string(centerX, ' ') << "Press Enter to exit..." << endl;

            cin.get(); // ����� �Է� ���
            break;
        }

        // ���� ��� ����
        if (isNext) {
            block = blocks[rand() % blockSize];
            cursorPos = { 0,mapWidth / 2 };
            isNext = false;
        }
        else {
            DeleteBeforePos(beforePos, block);
            // ���� �̸����� ��� ��ġ ����
            DeleteBeforePreviewPos(beforePreviewPos);
        }

        auto now = Clock::now();
        if (now - lastDropTime >= dropInterval) {
            if (!isBlockContactWall(cursorPos.first + 1, cursorPos.second, block) && !block.isContactedBlock(1)) {
                cursorPos.first += 1; // ����� �Ʒ��� ����
                DeleteBeforePos(beforePos, block); // ���� ��� ��ġ ����
            }
            else
                isNext = true; // ����� �� �̻� ������ �� ������ ���� ��� ����

            lastDropTime = now; // ������ ��� �ð� ������Ʈ
        }

        // Ű �Է�
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

        // todo ����� ��� ������ �� �̸����� ���
        OutputPreviewBlock(block, beforePreviewPos);

        DeleteLine();

        // ��� ��� ��ǥ ����
        PrintBlock(block, 0);
        // �� ���
        PrintMap();

        // �� �ʸ��� �������� ��
        this_thread::sleep_for(chrono::milliseconds(16)); // 60 ������
    }
    return 0;
}