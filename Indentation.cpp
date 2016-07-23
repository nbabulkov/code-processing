#include <iostream>

#include "Indentation.h"

Indentation::Indentation(char* command) : Command(command)
{}

Indentation::~Indentation()
{}

Indentation::Indentation(const Indentation& c) : Command(c)
{}

Indentation& Indentation::operator = (const Indentation& c)
{
	Command::operator = (c);
	return *this;
}


void Indentation::transformCode(bool& isWin)
{
	size_t lines = sCode.getSize();
	if (!strcmp(argument, "tabs"))
		allTabs(lines);
	else if (!strcmp(argument, "spaces"))
		allSpaces(lines);
	else  
		throw std::invalid_argument("No such argument!");
}

bool Indentation::isSpaces(char* curr, size_t amount = 4) // might be used by more classes
{
	for (size_t pos = 0; pos < amount; ++pos)
	{
		if (curr[pos] != ' ')
			return false;
	}
	return true;
}

void Indentation::replaceWithTabs(char* curr)
{
	strcpy(curr, curr + 3); 	// copies the rest of the string over 3 of the spaces
	*curr = '\t';				// the 1 space that remains is changed to a tab	
}

void Indentation::allTabs(size_t lines)
{
	for (size_t line = 0; line < lines; ++line)
	{
		size_t pos = 0;
		while (isSpaces(sCode[line] + pos))
		{
			replaceWithTabs(sCode[line] + pos);
			pos++;
		}
	}
}

void Indentation::replaceWithSpaces(size_t line, size_t pos)
{
	char* afterTab = sCode[line] + pos + 1;
	size_t size = strlen(sCode[line]) + 4;
	char* newLine = new char[size];
	strncpy(newLine, sCode[line], pos);
	strcpy(newLine + pos, "    ");
	strcat(newLine, afterTab);
	sCode[line] = newLine;
}

void Indentation::allSpaces(size_t lines)
{
	for (size_t line = 0; line < lines; ++line)
	{
		size_t pos = 0;
		while (sCode[line][pos] == '\t')
		{
			replaceWithSpaces(line, pos);
			pos += 4;
		}
	}
}