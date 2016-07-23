#include <iostream>
#include <assert.h>
#include "SourceCode.h"

SourceCode::SourceCode()
{
	lines = 0;
	code = NULL;
}

SourceCode::SourceCode(char* rawCode)
{
	lines = calcLines(rawCode);
	code = new char*[lines];
	format(rawCode);
}

SourceCode::~SourceCode()
{
	clear();
}

SourceCode::SourceCode(const SourceCode& s)
{
	copy(s);
}

SourceCode& SourceCode::operator = (const SourceCode& s)
{
	if (this != &s)
	{
		clear();
		copy(s);
	}
	return *this;
}

void SourceCode::clear()
{
	for (size_t line = 0; line < lines; ++line)
		delete[] code[line];
	delete[] code;
}

void SourceCode::copy(const SourceCode& s)
{
	lines = s.lines;
	code = new char*[lines];
	for (size_t line = 0; line < lines; ++line)
		strcpy(code[line], s.code[line]);
}

size_t SourceCode::calcLines(char* codeStr) const
{
	size_t line = 0;
	while (codeStr[line] != '\0')
	{
		if (codeStr[line] == '\n')
			line++;
	}
	return line + 1;
}

void SourceCode::format(char* rawCode)
{
	size_t sumOfPrev = 0,
		   line = 0;
			 
	for (size_t pos = 0; rawCode[pos - 1] != '\0'; ++pos)
	{
		assert(line < lines);
		if (rawCode[pos] == '\n')
		{
			size_t lineSize = pos - sumOfPrev ;
			code[line] = new char[lineSize];

			char* lineStart = rawCode + sumOfPrev;
			strncpy(code[line], lineStart, lineSize);
			code[line++][lineSize] = '\0';
			sumOfPrev = pos;
		}
		else if (rawCode[pos] == '\0')
		{
			assert(line == lines - 1);
			char* lineStart = rawCode + sumOfPrev;
			strcpy(code[line], lineStart);
		}
	}
}

size_t SourceCode::calcSize() const
{
	size_t size = 0;
	for (size_t line = 0; line < lines; ++line)
		size += strlen(code[line]);
	return size;
}

char* SourceCode::genRawCode(bool isWindowsStyle) const
{
	size_t endingChars = isWindowsStyle ? 2 : 1,
		size = calcSize() + endingChars * lines;
	char* rawCode = new char[size];

	size_t pos = 0;
	for (size_t line = 0; line < lines; ++line)
	{
		assert(pos < size);
		strcpy(rawCode + pos, code[line]);
		if (line < lines - 1)
		{
			pos += strlen(code[line]) + endingChars;
			if (isWindowsStyle)
			{
				rawCode[pos] = '\r';
				rawCode[pos - 1] = '\n';
			}
			else
				rawCode[pos] = '\n';
		}
	}
	return rawCode;
}

char** SourceCode::getCode() const
{
	return code;
}

inline size_t SourceCode::getLines() const
{
	return lines;
}
inline void SourceCode::setLines(size_t line)
{
	this->lines = lines;
}

inline void SourceCode::print() const
{
	for (size_t line = 0; line < lines; ++line)
		std::cout << code[line] << std::endl;
}