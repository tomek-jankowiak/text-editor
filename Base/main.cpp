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
	backend->bind(".Edition.Undo", [&tool]() {tool->undo(); }, "Undo changes");
	backend->bind(".Edition.Redo", [&tool]() {tool->redo(); }, "Redo changes");
	backend->bind(".Edition.Jump to${Line number:|lineNumber}", [&tool]() {tool->jumpTo(); }, "Jump to line");
	backend->bind(".Edition.Find${Searched phrase:|phrase}", [&tool]() {tool->find(); }, "Search phrase in text");
	backend->bind("<EDITION>", [&tool]() {tool->editionHandler(); }, "");
	backend->start();
	delete backend;
	delete tool;
}