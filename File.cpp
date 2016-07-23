#include <iostream>
#include <fstream>

#include "File.h"

using namespace std;

File::File()
{
	path = NULL;
}

File::File(const char* str)
{
	setPath(str);
}

File::~File()
{
	delete[] path;
}

File::File(const File& f)
{
	path = copyStr(f.path);
}

File& File::operator = (const File& f)
{
	if (this != &f)
	{
		delete[] path;
		path = copyStr(f.path);
	}
	return *this;
}

char* File::copyStr(const char* str)
{
	size_t size = strlen(str) + 1;
	char* newStr = new char[size];
	strcpy(newStr, str);
	return newStr;
}

void File::setPath(const char* str)
{
	path = copyStr(str);
}

char* File::readFromFile()
{
	std::ifstream input(path, ios::in | ios::binary);
	if (input.good())
	{
		input.seekg(0, ios::end);
		size_t size = input.tellg();
		input.seekg(0, ios::beg);

		char* content = new char[size + 1];
		input.read(content, size);
		content[size] = '\0';
		return content;
	}
	else
		throw std::exception("Couldn't open the file!");
	input.close();
}

void File::writeToFile(char* content)
{
	std::ofstream output(path, ios::out | ios::binary);
	if (output.good())
	{
		size_t size = strlen(content);
		output.write(content, size);
	}
	output.close();
}

inline void File::printPath() const
{
	cout << path << endl;
}