#pragma once

#include "CommentRemoval.h"

class Normalise: public Command
{
private:
	bool isWindosStyle;

public:
	Normalise(char*);
	virtual ~Normalise();

public:
	virtual void transformCode(bool&);
	bool getNLStyle() const;
};

