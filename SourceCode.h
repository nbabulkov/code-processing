#pragma once
#include "DynCode.h"

class SourceCode
{
private:
	DynCode code;	// dynamic array of lines

public:
	SourceCode();
	SourceCode(char*);
	SourceCode(const SourceCode&);
	SourceCode& operator =(const SourceCode&);

private:
	void clear();
	void copy(const SourceCode&);

	bool isEndOfLine(const char) const;

	void format(char*);
	size_t calcSize() const;

public: 
	char* genRawCode(bool) const;
	DynCode& getCode();
	void setCode(char*);


	void print() const;
};