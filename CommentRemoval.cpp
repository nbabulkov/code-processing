#include <iostream>
#include "CommentRemoval.h"


Comments::Comments() 
{}

Comments::~Comments()
{}

Comments::Comments(const Comments& c) : Command(c)
{}

Comments& Comments::operator = (const Comments& c)
{
	Command::operator = (c);
	return *this;
}


// function should go in parent class to be used by all children
void Comments::replace(size_t startLine, size_t pos, char* comFin)
{
	sCode[startLine][pos] = '\0';
	strcat(sCode[startLine], comFin);
}

void Comments::remove(size_t startLine, size_t startPos, size_t& line, size_t pos)
{
	char* comFin = sCode[line] + pos;

	// uniting the begining and the end of the comment
	replace(startLine, startPos, comFin);	
	pos = startPos;
	
	// removing commented lines
	while (startLine < line)				
		delete[] sCode.takeOut(line--);
}


void Comments::transformCode(bool& isWin)
{
	bool open = false;
	size_t startLine,
			startPos;

	for (size_t line = 0; line < sCode.getSize(); ++line)
	{
		for (size_t pos = 0; sCode[line][pos] != '\0'; ++pos)
		{
			if (!open && isCOpen(sCode[line] + pos))
			{
				open = true;
				startLine = line;
				startPos = pos;
			}
			else if (open && isCClosed(sCode[line] + pos))
			{
				open = false;
				remove(startLine, startPos, line, pos + 2);
				pos = startPos;
			}
			else if (isCppOpen(sCode[line] + pos))
			{
				sCode[line][pos] = '\0';
				break;
			}
		}
	}
	sCode.print();
}

