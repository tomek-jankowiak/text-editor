#include "Buffer.h"

Buffer::Buffer()
{
}

void Buffer::instertLine(std::string line, int n)
{
	line = removeTabs(line);
	lines.insert(lines.begin() + n, line);
}

void Buffer::appendLine(std::string line)
{
	line = removeTabs(line);
	lines.push_back(line);
}

void Buffer::removeLine(int n)
{
	lines.erase(lines.begin() + n);
}

std::string Buffer::removeTabs(std::string line)
{
	int tabPos = line.find("\t");
	while (tabPos != line.npos)
	{
		line.replace(tabPos, 1, "    ");
		tabPos = line.find("\t");
	}
	return line;
}

void Buffer::copyBuffer()
{
	bufferCopy.clear();
	for (int i = 0; i < lines.size(); i++)
		bufferCopy.push_back(lines[i]);
}

void Buffer::addToUndoStack(int curs_x, int curs_y)
{
	copyBuffer();
	undoChanges.push(bufferCopy);
	prevCursPos[0] = curs_x;
	prevCursPos[1] = curs_y;
	cursorPosChangesUndo.push(prevCursPos);
}

void Buffer::handleChange(int mode, int curs_x, int curs_y)
{
	currCursPos[0] = curs_x;
	currCursPos[1] = curs_y;
	// 0 - undo, 1 - redo
	switch (mode)
	{
	case 0:
		if (undoChanges.size() == 0)
			break;
		
		redoChanges.push(lines);
		cursorPosChangesRedo.push(currCursPos);
		lines.clear();
		for (int i = 0; i < undoChanges.top().size(); i++)
			lines.push_back(undoChanges.top()[i]);
		currCursPos[0] = cursorPosChangesUndo.top()[0];
		currCursPos[1] = cursorPosChangesUndo.top()[1];
		
		cursorPosChangesUndo.pop();
		undoChanges.pop();
		break;

	case 1:
		if (redoChanges.size() == 0)
			break;

		undoChanges.push(lines);
		cursorPosChangesUndo.push(currCursPos);
		lines.clear();
		for (int i = 0; i < redoChanges.top().size(); i++)
			lines.push_back(redoChanges.top()[i]);
		currCursPos[0] = cursorPosChangesRedo.top()[0];
		currCursPos[1] = cursorPosChangesRedo.top()[1];

		cursorPosChangesRedo.pop();
		redoChanges.pop();
		break;
	}
}