#pragma once
#include "Command.h"


// WORK IN PROGRESS...

enum Colour
{
	blue,
	green,
	orange,
	NONE
};


class Html: public Command
{
private:
	char** htmlCode;
	size_t codeSize;

public:
	Html();
	virtual ~Html();

public:
	virtual void transformCode(bool&);

private:
	void convertToGreen(size_t);
	void convertToOrange(size_t);
	void convertToBlue(size_t);
	char* convertLine(size_t);
	bool isSeparate(size_t, size_t, size_t);
	bool isStatement(size_t, size_t);
	Colour whatStatement(char*);
};

