#pragma once

#include <unordered_map>
#include <regex>

#include "BaseTool.h"
#include "Editor.h"
#include "ErrorAlert.h"

class TextEditorTool :
	public BaseTool
{
	std::unordered_map<std::string, std::string> entries;
	WINDOW* window;
	WINDOW* errorWindow;
	Editor* editor = 0;
	ErrorAlert alert;
	std::string fileName;
	std::string saveFileName;
	bool fileOpened = false;

	void assignWindow(WINDOW*) override;

public:
	TextEditorTool();
	~TextEditorTool();
	
	std::string getEntry(const std::string&) const override;
	void setEntry(const std::string&, const std::string&) override;

	void newFile();
	void openFile();
	void saveFile();
	void saveFileAs();
	void undo();
	void redo();
	void jumpTo();
	void find();
	void editionHandler();
};

