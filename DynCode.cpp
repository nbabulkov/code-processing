#include <iostream>
#include <assert.h>

#include "DynCode.h"


// Big 4

DynCode::DynCode()
{
	cap = 1;
	lineArr = new char*[cap];
	size = 0;
}

DynCode::DynCode(size_t _cap)
{
	cap = _cap;
	lineArr = new char*[cap];
	size = 0;
}

DynCode::~DynCode()
{
	clear();
}

DynCode::DynCode(const DynCode& d)
{
	copy(d);
}


DynCode& DynCode::operator = (const DynCode& d)
{
	if (this != &d)
	{
		clear();
		copy(d);
	}
	return *this;
}

// Gets

size_t DynCode::getSize() const
{
	return size;
}

size_t DynCode::getCap() const
{
	return cap;
}


// Help Functions


char* DynCode::copyStr(const char* curr)
{
	int size = strlen(curr) + 1;
	char* str = new char[size];
	strcpy(str, curr);
	return str;
}

void DynCode::copy(const DynCode& d)
{
	cap = d.cap;
	size = d.size;
	lineArr = new char*[cap];
	for (size_t line = 0; line < size; ++line)
	{
		size_t lineSize = strlen(d.lineArr[line]) + 1;
		lineArr[line] = new char[lineSize];
		strcpy(lineArr[line], d.lineArr[line]);
	}
}

void DynCode::clear()
{
	for (size_t i = 0; i < size; ++i)
		delete[] lineArr[i];
	delete[] lineArr;
}

void DynCode::newArr(size_t _cap)
{
	assert(size < _cap);
	cap = _cap;
	char** newArr = new char*[cap];
	for (size_t i = 0; i < size; ++i)
	{
		size_t lineSize = strlen(lineArr[i]) + 1;
		newArr[i] = new char[lineSize];
		strcpy(newArr[i], lineArr[i]);
	}
	clear();
	lineArr = newArr;
}

void DynCode::print() const
{
	for (size_t line = 0; line < size; ++line)
		std::cout << lineArr[line] << std::endl;
}


// Operators 

char*& DynCode::operator [](const int pos)
{
	if (pos < 0)
		throw std::invalid_argument("The position must be a non-negative intiger!");
	else if (pos >= size)
		throw std::out_of_range("The position must not exceed the size!");

	return lineArr[pos];
}

const char* DynCode::operator [] (const int pos) const
{
	if (pos < 0)
		throw std::invalid_argument("The position must be a non-negative intiger!");
	else if (pos >= size)
		throw std::out_of_range("The position must be a non-negative intiger!");
	return lineArr[pos];
}


 // Functions for array manipulation

void DynCode::incSize()
{
	assert(size == cap);
	cap *= 2;
	newArr(cap);
}


void DynCode::decSize()
{
	assert(size < cap / 2);
	cap /= 2;
	newArr(cap);
}

void DynCode::moveOneForward(size_t pos)
{
	for (int i = size; i > pos; --i)
		lineArr[i] = lineArr[i - 1];
}

void DynCode::moveOneBackwards(size_t pos)
{
	for (int i = pos; i < size - 1; ++i)
		lineArr[i] = lineArr[i + 1];
}


bool DynCode::isEmpty() const
{
	return size == 0 ? true : false;
}

void DynCode::putIn(char* elem)
{
	assert(size <= cap);
	lineArr[size++] = elem;
	if (size == cap)
		incSize();
}

void DynCode::putInBetween(char* elem, size_t pos)
{
	assert(size <= cap);
	if (pos > size)
		throw std::out_of_range("Position exceeds the size of the array!");
	else if (pos < size)
		moveOneForward(pos);
	lineArr[pos] = elem;
	size++;

	if (size == cap)
		incSize();
}

char* DynCode::takeOut(size_t pos)
{
	assert(size != 0);
	char* line = copyStr(lineArr[pos]);

	delete[] lineArr[pos];
	moveOneBackwards(pos);
	size--;

	if (size < cap / 2)
		decSize();

	return line;
}