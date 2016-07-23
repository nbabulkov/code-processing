#include <iostream>
#include "Html.h"

const size_t blueSize = 20;
const char blueC[blueSize][14] =
{
	"if", "for", "while", "do", "else",
	"const", "char", "int", "double", "float", "unsigned",
	"new", "delete"
};


Html::Html()
{
	codeSize = sCode.getSize() + 1;
	htmlCode = new char*[codeSize];
	for (size_t line = 0; line < codeSize; ++line)
		htmlCode[line] = NULL;
}

Html::~Html()
{
	for (size_t line = 0; line < codeSize; ++line)
		delete[] htmlCode[line];
	delete[] htmlCode;
}


void Html::convertToGreen(size_t line)
{

	
}

void Html::convertToOrange(size_t line)
{


}

void Html::convertToBlue(size_t line)
{


}

char* Html::convertLine(size_t line)
{
	for (size_t pos = 0; sCode[line][pos]; ++pos)
	{
		convertToGreen(line);
	}
	return "not done yet";
}

void Html::transformCode(bool& isWin)
{
	size_t lines = sCode.getSize();
	for (size_t line = 0; line < lines; ++line)
	{
		char* converted = convertLine(line);
		htmlCode[line] = converted;
	}
}