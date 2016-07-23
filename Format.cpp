#include <iostream>
#include "Format.h"

const size_t amountOfSt = 5;
const char statements[][7] = { "if", "while", "for", "else", "do" };

Format::Format()
{
	indentation = 0;
}

Format::~Format()
{}


// Bools

bool Format::isCppComment(char* curr) const
{
	return curr[0] == '/' && curr[1] == '/';
}

bool Format::isLetter(char symb) const
{
	return (symb >= 'a' && symb <= 'z') || (symb >= 'A' && symb <= 'Z');
}

bool Format::isLastSymbol(size_t line, size_t pos) const
{
	size_t end = skip(sCode[line], ' ', pos + 1);
	return sCode[line][end] == '\0';
}

bool Format::nextsIsScope(size_t line, size_t pos) const
{
	size_t currLinePos = skip(sCode[line], ' ', pos);
	size_t newLinePos = skip(sCode[line + 1], '\t', 0);
	bool scopeOnNextLine = sCode[line][currLinePos] == '\0' &&
		sCode[line + 1][newLinePos] != '{';

	return sCode[line][currLinePos] == '{' || scopeOnNextLine;
}

// Skiping functions

size_t Format::skip(char* currLine, char symb, size_t pos = 0) const
{
	while (currLine[pos] == symb)
		pos++;
	return pos;
}

size_t Format::skipLetters(size_t line, size_t pos) const
{
	while (isLetter(sCode[line][pos]))
		pos++;
	return pos;
}


void Format::skipBrackets(size_t line, size_t& pos)
{
	pos = skip(sCode[line], ' ', pos);
	int brackets = 0;
	while (sCode[line][pos] != '\0')
	{
		if (sCode[line][pos] == '(')
			brackets++;
		else if (sCode[line][pos] == ')')
		{
			brackets--;
			if (!brackets)
				return;
		}
		pos++;
	}
	pos = skip(sCode[line], ' ', pos);
}



// Line management

void Format::cutLine(size_t line)
{
	if (sCode[line][0] == '\0')
		sCode.takeOut(line);
	else
	{
		size_t size = strlen(sCode[line]) + 1;
		char* newLine = new char[size];
		strcpy(newLine, sCode[line]);
		delete[] sCode[line];
		sCode[line] = newLine;
	}
}

// if size == 0 copy the whole line,
// else copy the the given amount
char* Format::createNewLine(char* curr, size_t size = 0, int ind = 0) const
{
	ind += indentation;
	size_t sizeToEnd = size == 0 ? strlen(curr) + 1 : size + 1;
	char* newLine = new char[sizeToEnd + ind];
	for (size_t i = 0; i < ind; ++i)
		newLine[i] = '\t';
	if (size == 0)
		strcpy(newLine + ind, curr);
	else
		strncpy(newLine + ind, curr, sizeToEnd);
	return newLine;
}


// Scope management

void Format::openScope(size_t line, size_t pos)
{
	char* newLine = NULL;
	size_t endPos = skip(sCode[line], ' ', pos + 1);
	if (sCode[line][endPos] != '\0')
	{
		// new line for the chars after the bracket
		newLine = createNewLine(sCode[line] + pos + 1, 0, 1);
		sCode.putInBetween(newLine, line + 1);
		sCode[line][pos + 1] = '\0';
		cutLine(line);
	}

	if (pos != indentation)
	{
		// puts in the opening bracket
		newLine = createNewLine(sCode[line] + pos, 1, 1);
		strcpy(newLine, "{");
		sCode.putInBetween(newLine, line + 1);
		sCode[line][pos] = '\0';
		cutLine(line);
	}
}

void Format::closeScope(size_t line, size_t pos)
{
	char* newLine = NULL;
	size_t endPos = skip(sCode[line], ' ', pos + 1);
	if (sCode[line][endPos] != '\0')
	{
		newLine = createNewLine(sCode[line] + pos + 1);
		sCode.putInBetween(newLine, line + 1);
		sCode[line][pos + 1] = '\0';
	}

	// puts in the closing bracket
	newLine = new char[2];
	strcpy(newLine, "}");
	sCode.putInBetween(newLine, line + 1);
	sCode[line][pos] = '\0';
}


// Scope managment

void Format::formatScope(size_t& line) 
{
	char* codeLine = sCode[line];
	for (size_t pos = skip(codeLine, '\t'); codeLine[pos] != '\0'; ++pos)
	{
		if (codeLine[pos] == '{')
		{
			indentation++;
			openScope(line, pos);
			line++;
			break;
		}
		else if (codeLine[pos] == '}')
		{
			indentation--;
			closeScope(line, pos);
			line++;
			break;
		}
	}
}

void Format::adjustIndetation(size_t line)
{
	size_t lineInd = skip(sCode[line], '\t');
	if (indentation != lineInd)
	{
		// private: , public:, case: ....
		int difference = indentation - lineInd;
		int size = strlen(sCode[line]) + difference + 1;
		char* currLine = new char[size];
		if (difference < 0)
			strcpy(currLine, sCode[line] - difference);
		else if (difference > 0)
		{
			for (size_t i = 0; i < difference; ++i)
				currLine[i] = '\t';
			strcpy(currLine + difference, sCode[line]);
		}
		delete[] sCode[line];
		sCode[line] = currLine;
	}
}


void Format::formatWithoutScope(size_t line)
{
	for (size_t pos = skip(sCode[line], '\t'); sCode[line][pos] != '\0'; ++pos)
	{
		bool isPrevSpace = (pos == 0) ? true :
			sCode[line][pos - 1] == ' ' || sCode[line][pos - 1] == '\t';
		size_t wordEnd = skipLetters(line, pos);

		for (size_t i = 0; i < amountOfSt; ++i)
		{
			// is statement
			if (wordEnd - pos <= 0)
				break;
			bool isStatement = !strncmp(sCode[line] + pos, statements[i], wordEnd - pos);
			if (isStatement && isPrevSpace)
			{
				const char* st = statements[i];
				pos += strlen(st);
				skipBrackets(line, pos);
				if (!nextsIsScope(line, pos))
				{
					char* newLine = createNewLine(sCode[line] + pos + 1, 0, 1);
					sCode.putInBetween(newLine, line + 1);
					sCode[line][pos + 1] = '\0';
					cutLine(line);
				}
			}
		}
	}
}


// Main functions

void Format::formatStatements()
{
	for (size_t line = 0; line < sCode.getSize(); ++line)
	{
		for (size_t pos = 0; sCode[line][pos] != '\0'; ++pos)
		{
			if (sCode[line][pos] == ';' && !isLastSymbol(line, pos))
			{
				char* newLine = createNewLine(sCode[line] + pos + 1);
				sCode.putInBetween(newLine, line + 1);

				sCode[line][pos + 1] = '\0';
				cutLine(line);
				break;
			}
		}
	}
}

void Format::formatBlocks()
{
	for (size_t line = 0; line < sCode.getSize(); ++line)
	{
		formatScope(line);
		formatWithoutScope(line);
		adjustIndetation(line);
	}
}

void Format::transformCode(bool& isWin)
{
	formatStatements();
	formatBlocks();
}