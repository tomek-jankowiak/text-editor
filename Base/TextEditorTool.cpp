#include <exception>
#include "TextEditorTool.h"
#include "Editor.h"

TextEditorTool::TextEditorTool()
{
	this->window = nullptr;
}

TextEditorTool::~TextEditorTool()
{
	delete this->window;
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
	this->editor = new Editor(this->window);
	wclear(this->window);
}

void TextEditorTool::openFile()
{
	this->fileName = getEntry("fileName");
	this->editor = new Editor(this->window, this->fileName);
	wclear(this->window);
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

void TextEditorTool::editionHandler()
{
	std::string key = getEntry("KEY");
	if (key == "<RESIZE>")
	{
		this->editor->x = 0;
		this->editor->y = 0;
		this->editor->printBuff();
	}
	else
	{
		this->editor->handleInput(key);
		this->editor->printBuff();
	}
}