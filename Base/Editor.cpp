#include <fstream>
#include <iostream>

#include "Editor.h"

Editor::Editor(WINDOW* scr)
{
	window = scr;
	filename = "";
	buff = new Buffer();
	buff->appendLine("");

}

Editor::Editor(WINDOW* scr, std::string name)
{
	filename = name;
	window = scr;
	buff = new Buffer();

	std::fstream inputFile;
	inputFile.open(name.c_str(), std::ios::in);
	if (inputFile.is_open())
	{
		std::string temp;
		while (getline(inputFile, temp))
			buff->appendLine(temp);

		if (buff->lines.size() > LINES - 1)
			linesUnder = buff->lines.size() - (LINES - 1);
	}
	else
		throw FileNotFound();

	inputFile.close();
}

Editor::~Editor()
{
	delete buff;
}

void Editor::moveUp()
{
	if (linesAbove > 0)
	{
		if (y - linesAbove == 0)
		{
			linesAbove--;
			linesUnder++;
		}
		y--;		
	}
	else
	{
		if (y - 1 >= 0)
			y--;
	}
	if (x >= buff->lines[y].length())
		x = buff->lines[y].length();
	wmove(window, y - linesAbove, x);
}

void Editor::moveDown()
{
	if (linesUnder > 0)
	{
		if (y - linesAbove + 1 == LINES - 1)
		{
			linesAbove++;
			linesUnder--;
		}
		y++;
	}
	else
	{
		if (y + 1 < LINES - 1 && y + 1 < buff->lines.size())
			y++;
	}
	if (x >= buff->lines[y].length())
		x = buff->lines[y].length();
	wmove(window, y - linesAbove, x);
}

void Editor::moveLeft()
{
	if (x - 1 >= 0)
	{
		x--;
		wmove(window, y - linesAbove, x);
	}
}

void Editor::moveRight()
{
	if (x + 1 < COLS && x + 1 <= buff->lines[y].length())
	{
		x++;
		wmove(window, y - linesAbove, x);
	}
}

void Editor::handleUndoRedo(int mode)
{
	wordChange = false;
	buff->handleChange(mode, x, y);
	x = buff->currCursPos[0];
	y = buff->currCursPos[1];
	wmove(window, y - linesAbove, x);
}

void Editor::jumpToLine(int line)
{
	if (line <= buff->lines.size())
	{
		while (line < linesAbove)
		{
			linesAbove--;
			if (buff->lines.size() - linesAbove >= LINES - 1)
				linesUnder++;
		}

		while (line > (buff->lines.size() - linesUnder))
		{
			linesAbove++;
			linesUnder--;
		}
		y = line - 1;
	}
	else
		throw LineOutOfRange();

	x = 0;
	wmove(window, y - linesAbove, x);
}

void Editor::findPhrase(std::string phrase)
{
	for(int i=0;i<buff->lines.size();i++)
		if (buff->lines[i].find(phrase) != std::string::npos)
		{
			y = i;
			x = buff->lines[i].find(phrase);
			jumpToLine(y + 1);
			break;
		}
	wmove(window, y - linesAbove, x);
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
			buff->addToUndoStack(x, y - linesAbove);
			if (y == linesAbove)
				linesAbove--;
			if (linesUnder > 0)
				linesUnder--;
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
				buff->addToUndoStack(x, y - linesAbove);
			else if (buff->lines[y][x - 1] == ' ')
				wordChange = false;
			else if (!wordChange)
			{
				buff->addToUndoStack(x, y - linesAbove);
				wordChange = true;
			}
			buff->lines[y].erase(--x, 1);
		}
	}

	else if (key == "<DEL>")
	{
		if (x == buff->lines[y].length() && y != buff->lines.size() - 1)
		{
			buff->addToUndoStack(x ,y - linesAbove);
			if(linesUnder > 0)
				linesUnder--;
			buff->lines[y] += buff->lines[y + 1];
			deleteLine(y + 1);
		}
		else
		{
			if (buff->lines[y][x] == ' ' && !wordChange)
				buff->addToUndoStack(x, y - linesAbove);
			else if (buff->lines[y][x] == ' ')
				wordChange = false;
			else if (!wordChange)
			{
				buff->addToUndoStack(x, y - linesAbove);
				wordChange = true;
			}
			buff->lines[y].erase(x, 1);
		}
	}

	else if (key == "<ENTER>")
	{
		buff->addToUndoStack(x, y - linesAbove);
		if (linesUnder > 0)
			linesUnder++;
		else
			if (buff->lines.size() - linesAbove == LINES - 1)
				linesUnder++;
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
			buff->addToUndoStack(x, y - linesAbove);
		
		wordChange = false;
		buff->lines[y].insert(x, 1, ' ');
		x++;
	}

	else if (key == "<TAB>")
	{
		if(!wordChange)
			buff->addToUndoStack(x, y - linesAbove);
		
		wordChange = false;
		buff->lines[y].insert(x, 4, ' ');
		x += 4;
	}

	else
	{
		if (!wordChange)
		{
			buff->addToUndoStack(x, y - linesAbove);
			wordChange = true;
		}
		buff->lines[y].insert(x, 1, key[0]);
		x++;
	}
}

void Editor::printBuff(bool resize)
{
	if(resize)
		if (linesUnder > 0)
			linesUnder = buff->lines.size() - (LINES - 1);
	for (int i = linesAbove; i < (buff->lines.size() - linesUnder); i++)
			mvwprintw(window, i - linesAbove, 0, buff->lines[i].c_str());
	
	wmove(window, y - linesAbove, x);
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
	if (filename != "")
	{
		std::fstream f;
		f.open(filename.c_str(), std::ios::out);
		for (int i = 0; i < buff->lines.size(); i++)
			f << buff->lines[i] << std::endl;

		f.close();
	}
	else
		throw FileNotFound();
}

void Editor::saveFile(std::string saveAsFilename)
{
	std::fstream f;
	f.open(saveAsFilename.c_str(), std::ios::out);
	for (int i = 0; i < buff->lines.size(); i++)
		f << buff->lines[i] << std::endl;

	f.close();
}