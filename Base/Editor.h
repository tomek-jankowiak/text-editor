#pragma once

#include <curses.h>

#include "Buffer.h"

class Editor
{
private:
	Buffer* buff;
	std::string filename;
	WINDOW* window;

	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();

	void deleteLine();
	void deleteLine(int);

public:
	int x, y;

	Editor(WINDOW*);
	Editor(WINDOW*, std::string);
	~Editor();
	void handleInput(std::string);
	void printBuff();
	void saveFile();
	void saveFile(std::string);
};

