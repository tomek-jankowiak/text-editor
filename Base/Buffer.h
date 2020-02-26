#pragma once

#include <string>
#include <vector>
#include <stack>

class Buffer
{
	std::stack<std::vector<std::string>> undoChanges;
	std::stack<std::vector<std::string>> redoChanges;
	std::vector<std::string> bufferCopy;
	std::stack<int*> cursorPosChangesUndo;
	std::stack<int*> cursorPosChangesRedo;

	void copyBuffer();

public:
	Buffer();

	std::vector<std::string> lines;

	int cursorPos[2];

	void instertLine(std::string, int);
	void appendLine(std::string);
	void removeLine(int);
	std::string removeTabs(std::string);
	void addToUndoStack(int, int);
	void handleChange(int);
};

