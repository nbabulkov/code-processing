#include <iostream>
#include <assert.h>

#include "SourceCode.h"

SourceCode::SourceCode()
{

}

SourceCode::SourceCode(const SourceCode& s) : code(s.code)
{

}

SourceCode& SourceCode::operator =(const SourceCode& s)
{
	if (this != &s)
		code = s.code;
	return *this;
}

SourceCode::SourceCode(char* rawCode)
{
	format(rawCode);
}

bool SourceCode::isEndOfLine(const char symb) const
{
	return symb == '\n' || symb == '\r';
}

void SourceCode::format(char* rawCode)
{
	size_t sumOfPrev = 0;

	for (size_t pos = 0; rawCode[pos - 1] != '\0'; ++pos)
	{
		if (rawCode[pos] == '\n' || rawCode[pos] == '\0')
		{
			size_t lineSize = (rawCode[pos - 1] == '\r') ? pos - sumOfPrev : pos - sumOfPrev + 1;
			char* lineStart = rawCode + sumOfPrev;
			char* currLine = new char[lineSize];
			
			strncpy(currLine, lineStart, lineSize);
			currLine[lineSize - 1] = '\0';
			sumOfPrev = pos + 1;
			
			code.putIn(currLine);
		}
	}
	delete[] rawCode;
}

size_t SourceCode::calcSize() const
{
	size_t size = 0;
	for (size_t line = 0; line < code.getSize(); ++line)
		size += strlen(code[line]);
	return size;
}

char* SourceCode::genRawCode(bool isWindowsStyle) const
{
	size_t lines = code.getSize();
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
	
			rawCode[pos - 1] = '\n';
			if (isWindowsStyle)				
				rawCode[pos - 2] = '\r';
		}
	}
	return rawCode;
}

DynCode& SourceCode::getCode()
{
	return code;
}

void SourceCode::setCode(char* rawCode)
{
	format(rawCode);
}


void SourceCode::print() const
{
	for (size_t line = 0; line < code.getSize(); ++line)
		std::cout << code[line] << std::endl;
}