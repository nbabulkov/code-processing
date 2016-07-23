#include <iostream>

#include "Command.h"


Command::Command()
{
	argument = NULL;
	code = NULL;
}

Command::Command(char* command)
{
	argument = extractArgument(command);
	code = NULL;
}

Command::~Command()
{
	delete[] argument;
}

Command::Command(const Command& com)
{
	copy(com);
}

Command& Command::operator = (const Command& com)
{
	if (this != &com)
	{
		delete[] argument;
		delete code;
		copy(com);
	}
	return *this;
}


// copies

void Command::copy(const Command& com)
{
	argument = copyStr(com.argument);
	code = com.code;
}

char* Command::copyStr(const char* com)
{
	int size = strlen(com) + 1;
	char* str = new char[size];
	strcpy(str, com);
	return str;
}


char* Command::extractArgument(char* command)
{
	size_t size = strlen(command);
	for (size_t i = 0; i < size; ++i)
	{
		if (command[i] == '=')
		{
			char* argStart = command + i + 1;
			char* arg = new char[size - i + 1];
			strcpy(arg, argStart);
			return arg;
		}
	}
	return NULL;
}

void Command::setCode(SourceCode& _code)
{
	DynCode& trasferCode = _code.getCode();
	code = &trasferCode;
}

bool Command::isCOpen(const  char* curr) const
{
	return *curr == '/' && *(curr + 1) == '*';
}

bool Command::isCClosed(const  char* curr) const
{
	return *curr == '*' && *(curr + 1) == '/';
}

bool Command::isCppOpen(const  char* curr) const
{
	return *curr == '/' && *(curr + 1) == '/';
}
