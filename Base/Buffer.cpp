#include "Buffer.h"

Buffer::Buffer()
{
}

void Buffer::instertLine(std::string line, int n)
{
	line = this->removeTabs(line);
	this->lines.insert(lines.begin() + n, line);
}

void Buffer::appendLine(std::string line)
{
	line = this->removeTabs(line);
	this->lines.push_back(line);
}

void Buffer::removeLine(int n)
{
	this->lines.erase(lines.begin() + n);
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
