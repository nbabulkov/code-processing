#pragma once
#include "Command.h"

class Indentation : public Command
{
public:
	virtual void transformCode(bool&);

public:
	Indentation(char*);
	~Indentation();
	Indentation(const Indentation&);
	Indentation& operator = (const Indentation&);

private:
	void allTabs(size_t);
	void allSpaces(size_t);

private:
	char* findArgBeg();
	bool isSpaces(char*, size_t);
	void replaceWithTabs(char*);
	void replaceWithSpaces(size_t, size_t);
};