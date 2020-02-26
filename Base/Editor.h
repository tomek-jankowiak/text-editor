#pragma once

#include <curses.h>

#include "Buffer.h"

class Editor
{
private:
	int x = 0, y = 0;
	int cursorPos[2] = { 0 };
	int linesAbove = 0, linesUnder = 0;
	Buffer* buff;
	std::string filename;
	bool wordChange = false;

	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();

	void deleteLine();
	void deleteLine(int);

public:
	WINDOW* window;

	Editor(WINDOW*);
	Editor(WINDOW*, std::string);
	~Editor();

	void handleUndoRedo(int);
	void jumpToLine(int);
	void findPhrase(std::string);
	void handleInput(std::string);
	void printBuff(bool);
	void saveFile();
	void saveFile(std::string);

	class FileNotFound {};
	class LineOutOfRange {};
};

