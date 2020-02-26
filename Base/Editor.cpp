#include <fstream>
#include <iostream>
#include "Editor.h"

Editor::Editor(WINDOW* scr)
{
	x = 0;
	y = 0;
	window = scr;
	filename = "";
	buff = new Buffer();
	buff->appendLine("");

}

Editor::Editor(WINDOW* scr, std::string name)
{
	x = 0;
	y = 0;
	filename = name;
	window = scr;

	buff = new Buffer();

	std::fstream inputFile;
	inputFile.open(name.c_str(), std::ios::in);

	std::string temp;
	while (getline(inputFile, temp))
		buff->appendLine(temp);

	inputFile.close();
}

Editor::~Editor()
{
	delete buff;
	delete window;
}

void Editor::moveUp()
{
	if (y - 1 >= 0)
		y--;
	if (x >= buff->lines[y].length())
		x = buff->lines[y].length();
	wmove(window, y, x);
}

void Editor::moveDown()
{
	if (y + 1 < LINES - 1 && y + 1 < buff->lines.size())
		y++;
	if (x >= buff->lines[y].length())
		x = buff->lines[y].length();
	wmove(window, y, x);
}

void Editor::moveLeft()
{
	if (x - 1 >= 0)
	{
		x--;
		wmove(window, y, x);
	}
}

void Editor::moveRight()
{
	if (x + 1 < COLS && x + 1 <= buff->lines[y].length())
	{
		x++;
		wmove(window, y, x);
	}
}

void Editor::handleUndoRedo(int mode)
{
	wordChange = false;
	buff->handleChange(mode);
	x = buff->cursorPos[0];
	y = buff->cursorPos[1];
	wmove(window, y, x);
}

void Editor::jumpToLine(int l)
{
	if (l < buff->lines.size())
		y = l - 1;
	else
		y = buff->lines.size() - 1;

	x = 0;
	wmove(window, l - 1, x);
}

void Editor::handleInput(std::string key)
{
	if (key == "<UARROW>")
	{
		if (wordChange)
			wordChange = false;
		moveUp();
	}

	else if (key == "<DARROW>")
	{
		if (wordChange)
			wordChange = false;
		moveDown();
	}

	else if (key == "<LARROW>")
	{
		if (wordChange)
			wordChange = false;
		moveLeft();
	}

	else if (key == "<RARROW>")
	{
		if (wordChange)
			wordChange = false;
		moveRight();
	}

	else if (key == "<BS>")
	{
		if (x == 0 && y > 0)
		{
			buff->addToUndoStack(x, y);
			x = buff->lines[y - 1].length();
			buff->lines[y - 1] += buff->lines[y];
			deleteLine();
			moveUp();
		}
		else if (x == 0 && y == 0)
			return;
		else
		{
			if (buff->lines[y][x - 1] == ' ' && !wordChange)
				buff->addToUndoStack(x, y);
			else if (buff->lines[y][x - 1] == ' ')
				wordChange = false;
			else if (!wordChange)
			{
				buff->addToUndoStack(x, y);
				wordChange = true;
			}
			buff->lines[y].erase(--x, 1);
		}
	}

	else if (key == "<DEL>")
	{
		if (x == buff->lines[y].length() && y != buff->lines.size() - 1)
		{
			buff->addToUndoStack(x ,y);
			buff->lines[y] += buff->lines[y + 1];
			deleteLine(y + 1);
		}
		else
		{
			if (buff->lines[y][x] == ' ' && !wordChange)
				buff->addToUndoStack(x, y);
			else if (buff->lines[y][x] == ' ')
				wordChange = false;
			else if (!wordChange)
			{
				buff->addToUndoStack(x, y);
				wordChange = true;
			}
			buff->lines[y].erase(x, 1);
		}
	}

	else if (key == "<ENTER>")
	{
		buff->addToUndoStack(x, y);
		wordChange = false;
		if (x < buff->lines[y].length())
		{
			buff->instertLine(buff->lines[y].substr(x, buff->lines[y].length() - x), y + 1);
			buff->lines[y].erase(x, buff->lines[y].length() - x);
		}
		else
			buff->instertLine("", y + 1);
		
		x = 0;
		moveDown();
	}

	else if (key == "<SPACE>")
	{
		if (!wordChange)
			buff->addToUndoStack(x, y);
		
		wordChange = false;
		buff->lines[y].insert(x, 1, ' ');
		x++;
	}

	else if (key == "<TAB>")
	{
		if(!wordChange)
			buff->addToUndoStack(x, y);
		
		wordChange = false;
		buff->lines[y].insert(x, 4, ' ');
		x += 4;
	}

	else if (key == "<CTRL+Z>")
		buff->handleChange(0);
	
	else if (key == "<CTRL+Y>")
		buff->handleChange(1);

	else
	{
		if (!wordChange)
		{
			buff->addToUndoStack(x, y);
			wordChange = true;
		}
		buff->lines[y].insert(x, 1, key[0]);
		x++;
	}
}

void Editor::printBuff()
{
	for (int i = 0; i < LINES - 1; i++)
	{
		if (i >= buff->lines.size())
			wmove(window, i, 0);
		else
			mvwprintw(window, i, 0, buff->lines[i].c_str());
	}
	wmove(window, y, x);
}

void Editor::deleteLine()
{
	buff->removeLine(y);
}

void Editor::deleteLine(int i)
{
	buff->removeLine(i);
}

void Editor::saveFile()
{
	std::fstream f;
	f.open(filename.c_str(), std::ios::out);
	for (int i = 0; i < buff->lines.size(); i++)
		f << buff->lines[i] << std::endl;

	f.close();
}

void Editor::saveFile(std::string saveAsFilename)
{
	std::fstream f;
	f.open(saveAsFilename.c_str(), std::ios::out);
	for (int i = 0; i < buff->lines.size(); i++)
		f << buff->lines[i] << std::endl;

	f.close();
}