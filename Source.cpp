
#pragma once

#include "Command.h"

class Format : public Command
{
private:
	size_t indentation;
	bool isTab;
	//size_t line;

public:
	virtual void transformCode();

public:
	Format();
	virtual ~Format();

private:
	void formatSemicolons(size_t);
	void formatScopes(size_t&);
	void formatStatements(size_t&);
	void adjustIndetation(size_t);

private:
	void openScope(size_t, size_t);
	void closeScope(size_t, size_t);
	bool nextsIsScope(size_t, size_t) const;
	void formatCurrStatement(size_t, size_t, const char*);
	void skipBrackets(size_t, size_t&);
	bool isCppComment(char*) const;
	bool isTabs() const;
	size_t skip(char*, char, size_t) const;
	size_t skipTo(char*, char, size_t) const;
	void cutLine(size_t);
	char* createNewLine(char*, size_t) const;
};


//----------------------------------------------------------------------------------------------

#include <iostream>
#include "Format.h"

const size_t amountOfSt = 5;
const char statements[][7] = { "if", "while", "for", "else", "do" };

Format::Format()
{
	indentation = 0;
	isTab = isTabs();
}

Format::~Format()
{}

bool Format::isTabs() const
{

	// ...	
	return true;
}

// if size == 0 copy the whole line,
// else copy the the given amount
char* Format::createNewLine(char* curr, size_t size = 0) const
{
	size_t sizeToEnd = size == 0 ? strlen(curr) + 1 : size + 1;
	char* newLine = new char[sizeToEnd + indentation];
	for (size_t i = 0; i < indentation; ++i)
		newLine[i] = '\t';
	if (size == 0)
		strcpy(newLine + indentation, curr);
	else
		strncpy(newLine + indentation, curr, sizeToEnd);
	return newLine;
}

size_t Format::skip(char* currLine, char symb, size_t pos = 0) const
{
	while (currLine[pos] == symb)
		pos++;
	return pos;
}

size_t Format::skipTo(char* currLine, char symb, size_t pos = 0) const
{
	if (currLine[0] == symb)
		return pos;
	while (currLine[pos++] != symb)
		;
	return pos;
}

bool Format::isCppComment(char* curr) const
{
	return curr[0] == '/' && curr[1] == '/';
}

void Format::formatSemicolons(size_t line)
{
	char* codeLine = sCode[line];
	for (size_t pos = skip(codeLine, '\t'); codeLine[pos] != '\0'; ++pos)
	{
		// semicolons in for
		bool isWord = codeLine[pos + 4] == ' ' || codeLine[pos + 4] == '(';
		
		// semicolons in comment
		if (isCppComment(codeLine + pos))
			break;

		if (codeLine[pos] == ';')
		{
			size_t endPos = skip(codeLine, ' ', pos + 1);
			if (codeLine[endPos] != '\0')
			{
				char* newLine = createNewLine(codeLine + endPos);
				codeLine[pos + 1] = '\0';
				cutLine(line);
				sCode.putInBetween(newLine, line + 1);
			}
			break;
		}
	}
}

// other case: the indentations are spaces
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

void Format::skipBrackets(size_t line, size_t& pos)
{
	pos = skip(sCode[line], ' ', pos);
	int brackets = 0;
	while (1)
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

bool Format::nextsIsScope(size_t line, size_t pos) const
{
	size_t newLinePos = skip(sCode[line + 1], '\t');
	bool scopeOnNextLine = sCode[line][pos] == '\0' &&
		sCode[line + 1][newLinePos] != '{';

	return sCode[line][pos] != '{' || scopeOnNextLine;
}

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

void Format::openScope(size_t line, size_t pos)
{
	char* newLine = NULL;
	size_t endPos = skip(sCode[line], ' ', pos + 1);
	if (sCode[line][endPos] != '\0')
	{
		// the chars after the bracket
		indentation++;
		newLine = createNewLine(sCode[line] + pos + 1);
		sCode.putInBetween(newLine, line + 1);
		indentation--;
	}

	// puts in the opening bracket
	newLine = createNewLine(sCode[line] + pos, 1);
	sCode.putInBetween(newLine, line + 1);
	sCode[line][pos] = '\0';
	cutLine(line);
}

void Format::closeScope(size_t line, size_t pos)
{
	char* newLine = NULL;
	size_t endPos = skip(sCode[line], ' ', pos);
	if (sCode[line][endPos] != '\0')
	{
		newLine = createNewLine(sCode[line] + pos + 1);
		sCode.putInBetween(newLine, line + 1);
	}

	// puts in the closing bracket
	newLine = createNewLine(sCode[line] + pos, 1);
	sCode.putInBetween(newLine, line + 1);
}

void Format::formatScopes(size_t& line)
{
	char* codeLine = sCode[line];
	for (size_t pos = skip(codeLine, '\t'); codeLine[pos] != '\0'; ++pos)
	{
		if (codeLine[pos] == '{')
		{
			if (pos > indentation)
				openScope(line, pos);
			line++;
			indentation++;
			break;
		}
		else if (codeLine[pos] == '}')
		{
			indentation--;
			if (pos > indentation)
				closeScope(line, pos);
			line++;
			break;
		}
	}
}

void Format::formatCurrStatement(size_t line, size_t pos, const char* st)
{
	pos += strlen(st);
	skipBrackets(line, pos);
	if (!nextsIsScope(line, pos))
	{
		if (indentation + 1 != skip(sCode[line + 1], '\t'))
		{
			indentation++;
			char* newLine = createNewLine(sCode[line] + pos);
			sCode.putInBetween(newLine, line + 1);
			indentation--;
		}
	}
}

void Format::formatStatements(size_t& line)
{
	char* codeLine = sCode[line];

	for (size_t pos = skip(sCode[line], '\t'); sCode[line][pos] != '\0'; ++pos)
	{
		// isn't in other word
		bool isPrevSpace = (pos == 0) ? true :
			sCode[line][pos - 1] == ' ' || sCode[line][pos - 1] == '\t';
		// 
		for (size_t i = 0; i < amountOfSt; ++i)
		{
			// is statement
			bool isStatement = !strncmp(sCode[line], statements[i], strlen(statements[i]));
			if (isStatement && isPrevSpace)
				formatCurrStatement(line, pos, statements[i]);
		}
	}
}

void Format::transformCode()
{
	// CHECK - if there isn't a newline after ';' insert one
	// if newlines are more than one cut them (OPT)

	// line could be a member
	for (size_t line = 0; line < sCode.getSize(); ++line)
	{
		formatScopes(line);
		if (line == sCode.getSize())
			break;
		formatSemicolons(line);
		adjustIndetation(line);
		//formatStatements(line);
		std::cout << sCode[line] << std::endl;
	}
}