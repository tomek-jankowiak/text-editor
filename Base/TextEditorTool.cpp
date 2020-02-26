#include <exception>
#include <iostream>
#include <cstdlib>

#include "TextEditorTool.h"
#include "Editor.h"

TextEditorTool::TextEditorTool()
{
	window = nullptr;
	errorWindow = nullptr;
}

TextEditorTool::~TextEditorTool()
{
	delete editor;
}

std::string TextEditorTool::getEntry(const std::string& key) const
{
	try {
		return entries.at(key);
	}
	catch (std::out_of_range&) {
		throw BaseTool::UndefinedEntry();
	}
}

void TextEditorTool::setEntry(const std::string& key, const std::string& value)
{
	entries[key] = value;
}

void TextEditorTool::assignWindow(WINDOW* windowptr)
{
	window = windowptr;
}

void TextEditorTool::newFile()
{
	if (editor)
		delete editor;
	editor = new Editor(window);
	wclear(window);
	wrefresh(window);
}

void TextEditorTool::openFile()
{
	if (editor)
		delete editor;
	fileName = getEntry("fileName");
	try
	{
		editor = new Editor(window, fileName);
		fileOpened = true;
		wclear(window);
		wrefresh(window);
		editor->printBuff(false);
	}
	catch (Editor::FileNotFound)
	{
		wclear(window);
		errorWindow = alert.alertMsg(window, "Wrong file name!");
		return;
	}
}

void TextEditorTool::saveFile()
{
	try
	{
		editor->saveFile();
	}
	catch (Editor::FileNotFound)
	{
		errorWindow = alert.alertMsg(window, "File untitled! Try Save As instead");
		return;
	}
	setEntry("IS_SAVED", "YES");
	
}

void TextEditorTool::saveFileAs()
{
	saveFileName = getEntry("fileName");
	editor->saveFile(saveFileName);
	setEntry("IS_SAVED", "YES");
}

void TextEditorTool::undo()
{
	wclear(window);
	editor->handleUndoRedo(0);
	editor->printBuff(false);
	wrefresh(window);
}

void TextEditorTool::redo()
{
	wclear(window);
	editor->handleUndoRedo(1);
	editor->printBuff(false);
	wrefresh(window);
}

void TextEditorTool::jumpTo()
{
	try
	{
		int line = atoi(getEntry("lineNumber").c_str());
		editor->jumpToLine(line);
		wclear(window);
		editor->printBuff(false);
		wrefresh(window);
	}
	catch (Editor::LineOutOfRange)
	{
		errorWindow = alert.alertMsg(window, "Line number is too high! Try again");
		return;
	}
}

void TextEditorTool::find()
{
	std::string phrase = getEntry("phrase");
	wclear(window);
	editor->findPhrase(phrase);
	editor->printBuff(false);
	wrefresh(window);
}

void TextEditorTool::editionHandler()
{
	if (errorWindow)
	{
		wclear(errorWindow);
		wrefresh(errorWindow);
		delwin(errorWindow);
		errorWindow = nullptr;
		return;
	}
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

		editor->printBuff(false);
		//editor->x = tempX;
		//editor->y = tempY;
		wmove(window, tempY, tempX);
	}
	else if (key == "<RESIZE>")
	{
		if (editor)
		{
			editor->window = window;
			editor->printBuff(true);
		}
	}
	else if (key == "<CTRL+Z>")
	{
		editor->handleUndoRedo(0);
		editor->printBuff(false);
	}
	else if (key == "<CTRL+Y>")
	{
		editor->handleUndoRedo(1);
		editor->printBuff(false);
	}
	else
	{
		setEntry("IS_SAVED", "NO");
		editor->handleInput(key);
		editor->printBuff(false);
	}

	wchgat(window, 1, A_ATTRIBUTES, COLOR_WHITE, NULL);
	wrefresh(window);
}