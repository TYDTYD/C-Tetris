#pragma once

enum Dir {
	Down,
	Left,
	Right,
	Rotate,
	Space,
	None,
};

// Ű���� �Է�
Dir HandleKeyInput();

enum Color
{
	Red = 12,
	Black = 0,
	Yellow = 14,
};

void SetCursorPosition(int x, int y);
void SetCursorOnOff(bool visible);
void SetCursorColor(Color color, Color bg = Color::Black);