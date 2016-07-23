#pragma once

#include "Command.h"

class Format: public Command
{
private:
	size_t indentation;

public:
	virtual void transformCode(bool&);

public:
	Format();
	virtual ~Format();

private:
	void formatBlocks();
	void formatStatements();

private:
	void formatWithoutScope(size_t);
	void formatScope(size_t&);
	void adjustIndetation(size_t);

private:
	bool isLetter(char)const;
	bool isLastSymbol(size_t, size_t)const;
	bool nextsIsScope(size_t, size_t) const;
	bool isCppComment(char*) const;

	size_t skip(char*, char, size_t) const;
	size_t skipLetters(size_t, size_t) const;
	void skipBrackets(size_t, size_t&);

	char* createNewLine(char*, size_t, int) const; 
	void cutLine(size_t);

	void openScope(size_t, size_t);
	void closeScope(size_t, size_t);
};

