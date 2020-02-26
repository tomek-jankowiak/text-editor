#include <exception>
#include <iostream>
#include <cstdlib>

#include "TextEditorTool.h"
#include "Editor.h"

TextEditorTool::TextEditorTool()
{
	this->window = nullptr;
}

TextEditorTool::~TextEditorTool()
{
	delete this->editor;
}

std::string TextEditorTool::getEntry(const std::string& key) const
{
	try {
		return this->entries.at(key);
	}
	catch (std::out_of_range&) {
		throw BaseTool::UndefinedEntry();
	}
}

void TextEditorTool::setEntry(const std::string& key, const std::string& value)
{
	this->entries[key] = value;
}

void TextEditorTool::assignWindow(WINDOW* windowptr)
{
	this->window = windowptr;
}

void TextEditorTool::newFile()
{
	if (editor)
		delete editor;
	this->editor = new Editor(this->window);
	wclear(this->window);
	wrefresh(window);
}

void TextEditorTool::openFile()
{
	if (editor)
		delete editor;
	this->fileName = getEntry("fileName");
	this->editor = new Editor(this->window, this->fileName);
	fileOpened = true;
	wclear(this->window);
	wrefresh(window);
	this->editor->printBuff();
}

void TextEditorTool::saveFile()
{
	this->editor->saveFile();
}

void TextEditorTool::saveFileAs()
{
	this->saveFileName = getEntry("fileName");
	this->editor->saveFile(this->saveFileName);
	this->setEntry("IS_SAVED", "YES");
}

void TextEditorTool::undo()
{
	wclear(window);
	editor->handleUndoRedo(0);
	editor->printBuff();
	wrefresh(window);
}

void TextEditorTool::redo()
{
	wclear(window);
	editor->handleUndoRedo(1);
	editor->printBuff();
	wrefresh(window);
}

void TextEditorTool::jumpTo()
{
	int line = atoi(getEntry("lineNumber").c_str());
	editor->jumpToLine(line);
	wrefresh(window);
}

void TextEditorTool::editionHandler()
{
	wclear(window);
	std::string key = getEntry("KEY");
	std::smatch result;
	std::regex cords("<(\\d+),(\\d+)>");
	if (!fileOpened)
	{
		editor = new Editor(window);
		fileOpened = true;
	}
	else if (std::regex_match(key, result, cords))
	{
		int tempX, tempY;

		tempX = atoi(key.substr(1, key.find(",") - 1).c_str());
		tempY = atoi(key.substr(key.find(",") + 1, (key.length() - key.find(",") - 2)).c_str());

		editor->printBuff();
		//editor->x = tempX;
		//editor->y = tempY;
		wmove(window, tempY, tempX);
	}
	else if (key == "<RESIZE>")
	{
		if (editor)
		{
			this->editor->window = this->window;
			this->editor->printBuff();
		}
	}
	else if (key == "<CTRL+Z>")
		editor->handleUndoRedo(0);

	else if (key == "<CTRL+Y>")
		editor->handleUndoRedo(1);
	else
	{
		this->editor->handleInput(key);
		this->editor->printBuff();
	}
	wrefresh(window);
}