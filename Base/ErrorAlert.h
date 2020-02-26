#pragma once
#include <curses.h>
#include <string>
class ErrorAlert
{
public:
	WINDOW* alertMsg(WINDOW*, std::string);
};

