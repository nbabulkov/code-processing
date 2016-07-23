#pragma once
#include "SourceCode.h"

#ifndef sCode
#define sCode (*code)
#endif

class Command
{
protected:
	char* argument;
	DynCode* code;
	
public:
	Command();
	Command(char*);
	virtual ~Command();
	Command(const Command&);
	Command& operator = (const Command&);

public:
	void setCode(SourceCode&);
	virtual void transformCode(bool&) = 0;

private:
	char* extractArgument(char*);

	void copy(const Command&);
	char* copyStr(const char*);

protected:
	bool isCOpen(const  char*) const;
	bool isCClosed(const  char*) const;
	bool isCppOpen(const  char*) const;

};
