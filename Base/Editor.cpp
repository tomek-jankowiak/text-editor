#include <fstream>
#include <iostream>
#include "Editor.h"

Editor::Editor(WINDOW* scr)
{
	this->window = scr;
	this->filename = "";
	this->buff = new Buffer();
}

Editor::Editor(WINDOW* scr, std::string name)
{
	this->x = 0;
	this->y = 0;
	this->filename = name;
	this->window = scr;

	this->buff = new Buffer();

	std::fstream inputFile;
	inputFile.open(name.c_str(), std::ios::in);

	while (!inputFile.eof())
	{
		std::string temp;
		inputFile >> temp;
		this->buff->appendLine(temp);
	}

	inputFile.close();
}

Editor::~Editor()
{
	delete this->buff;
	delete this->window;
}

void Editor::moveUp()
{
	if (this->y - 1 >= 0)
		this->y--;
	if (this->x >= this->buff->lines[this->y].length())
		this->x = this->buff->lines[this->y].length();
	wmove(this->window, this->y, this->x);
}

void Editor::moveDown()
{
	if (this->y + 1 < LINES - 1 && this->y + 1 < this->buff->lines.size())
		this->y++;
	if (this->x >= this->buff->lines[this->y].length())
		this->x = this->buff->lines[this->y].length();
	wmove(this->window, this->y, this->x);
}

void Editor::moveLeft()
{
	if (this->x - 1 >= 0)
	{
		this->x--;
		wmove(this->window, this->y, this->x);
	}
}

void Editor::moveRight()
{
	if (this->x + 1 < COLS && this->x + 1 <= this->buff->lines[this->y].length())
	{
		this->x++;
		wmove(this->window, this->y, this->x);
	}
}

void Editor::handleInput(std::string key)
{
	if (key == "<UARROW>")
		this->moveUp();

	else if (key == "<DARROW>")
		this->moveDown();

	else if (key == "<LARROW>")
		this->moveLeft();

	else if (key == "<RARROW>")
		this->moveRight();

	else if (key == "<BS>")
	{
		if (this->x == 0 && this->y > 0)
		{
			x = this->buff->lines[y - 1].length();
			this->buff->lines[y - 1] += this->buff->lines[y];
			this->deleteLine();
			this->moveUp();
		}
		else
			this->buff->lines[y].erase(--this->x, 1);
	}

	else if (key == "<DEL>")
	{
		if (this->x == this->buff->lines[y].length() && this->y != this->buff->lines.size() - 1)
		{
			this->buff->lines[y] += this->buff->lines[y + 1];
			this->deleteLine(this->y + 1);
		}
		else
			this->buff->lines[y].erase(x, 1);
	}

	else if (key == "<ENTER>")
	{
		if (this->x < this->buff->lines[y].length())
		{
			this->buff->instertLine(this->buff->lines[y].substr(this->x, this->buff->lines[y].length() - this->x), this->y + 1);
			this->buff->lines[y].erase(this->x, this->buff->lines[y].length() - this->x);
		}
		else
			this->buff->instertLine("", this->y + 1);
		
		this->x = 0;
		this->moveDown();
	}

	else if (key == "<SPACE>")
	{
		this->buff->lines[this->y].insert(this->x, 1, ' ');
		this->x++;
	}

	else if (key == "<TAB>")
	{
		this->buff->lines[this->y].insert(this->x, 4, ' ');
		this->x += 4;
	}

	else
	{
		char sign[1];
		strcpy_s(sign, key.c_str());
		this->buff->lines[this->y].insert(this->x, 1, sign[0]);
		this->x++;
	}

}

void Editor::printBuff()
{
	for (int i = 1; i < LINES - 1; i++)
	{
		if (i >= this->buff->lines.size())
			wmove(this->window, i, 0);
		else
			mvwprintw(this->window, i, 0, this->buff->lines[i].c_str());
	}
	wmove(this->window, this->y, this->x);
}

void Editor::deleteLine()
{
	this->buff->removeLine(this->y);
}

void Editor::deleteLine(int i)
{
	this->buff->removeLine(i);
}

void Editor::saveFile()
{
	std::fstream f;
	f.open(this->filename.c_str(), std::ios::out);
	for (int i = 0; i < this->buff->lines.size(); i++)
		f << this->buff->lines[i] << std::endl;

	f.close();
}

void Editor::saveFile(std::string saveAsFilename)
{
	std::fstream f;
	f.open(saveAsFilename.c_str(), std::ios::out);
	for (int i = 0; i < this->buff->lines.size(); i++)
		f << this->buff->lines[i] << std::endl;

	f.close();
}