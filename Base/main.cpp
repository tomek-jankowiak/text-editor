#include "../mcedit/MceditBackend.h"
#include "TextEditorTool.h"

int main()
{
	TextEditorTool *tool = new TextEditorTool();
	MceditBackend *backend = new MceditBackend(tool);

	backend->bind(".File.New", [&tool]() {tool->newFile(); }, "Create new empty file");
	backend->bind(".File.Open${File name:|fileName}", [&tool]() {tool->openFile(); }, "Open existing file");
	backend->bind(".File.Save", [&tool]() {tool->saveFile(); }, "Save file");
	backend->bind(".File.Save as${File name:|fileName}", [&tool]() {tool->saveFileAs(); }, "Save file as <path>");
	backend->bind("<EDITION>", [&tool]() {tool->editionHandler(); }, "");
	backend->start();
	delete backend;
	delete tool;
}