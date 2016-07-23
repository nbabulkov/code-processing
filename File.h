#pragma once 

#include "SourceCode.h"

class File
{
protected:
	char* path;

public:
	File();
	File(const char*);
	~File();
	File(const File&);
	File& operator = (const File&);

public:
	char* readFromFile();
	void writeToFile(char*);

public:
	void setPath(const char*);
	inline void printPath() const;

private:
	char* copyStr(const char*);
};