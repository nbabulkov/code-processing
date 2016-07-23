#pragma once

#include "Command.h"

class Comments: public Command
{

public:
	virtual void transformCode(bool&);

public:
	Comments();
	virtual ~Comments();
	Comments(const Comments&);
	Comments& operator = (const Comments&);

private:
	void replace(size_t, size_t, char*);
	void shortenLines(size_t, size_t);
	void remove(size_t, size_t, size_t&, size_t);

};

// skips the symbols for ending "*/"
// changes the size of the string, if it is cut
// goes to the begining of a new line