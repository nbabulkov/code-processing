#pragma once

class DynCode
{
private:
	size_t size;
	size_t cap;
	char** lineArr;

public:
	DynCode();
	DynCode(size_t);
	~DynCode();
	DynCode(const DynCode&);
	DynCode& operator = (const DynCode&);

public:
	size_t getSize() const;
	size_t getCap() const;

private:
	void copy(const DynCode&);
	void clear();
	char* copyStr(const char*);

private:
	void incSize();
	void decSize();
	void moveOneForward(size_t);
	void moveOneBackwards(size_t);
	void newArr(size_t);

public:
	char*& operator [] (const int);
	const char* operator [] (const int) const;

public:
	bool isEmpty() const;
	void putIn(char*);
	void putInBetween(char*, size_t);
	char* takeOut(size_t);

public:
	void print() const;
};
