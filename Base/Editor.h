#pragma once

#include <curses.h>

#include "Buffer.h"

class Editor
{
private:
	int x, y;
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
	void handleInput(std::string);
	void printBuff();
	void saveFile();
	void saveFile(std::string);
};

