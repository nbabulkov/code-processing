#include <iostream>
#include "Normalise.h"


Normalise::Normalise(char* command) : Command(command)	
{
}

Normalise::~Normalise()
{
}

// it is used when writing in file
// to determine whether if a "/r/n" or "/n"
// should be put in place of the terminating symbol
void Normalise::transformCode(bool& isWin)
{
	if (!strcmp(argument, "CRLF"))
		isWin = true;
	else if (!strcmp(argument, "LF"))
		isWin = false;
	else
		throw std::invalid_argument("No such argument!");
}

bool Normalise::getNLStyle() const
{
	return isWindosStyle;
}