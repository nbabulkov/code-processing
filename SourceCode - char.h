#pragma once

class SourceCode
{
private:
	char** code; 
	size_t lines;

public:
	SourceCode();
	SourceCode(char*);
	~SourceCode();
	SourceCode(const SourceCode&);
	SourceCode& operator = (const SourceCode&);

private:
	void clear();
	void copy(const SourceCode&);

	size_t calcLines(char*) const;
	void format(char*);

	size_t calcSize() const;
	char* genRawCode(bool) const;
public: 
	inline char** getCode() const;
	inline size_t getLines() const;

	inline void setLines(size_t);

	inline void print() const;
};