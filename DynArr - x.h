#pragma once

#include <iostream>
#include <assert.h>

class DynArr
{
private:
	size_t size;
	size_t cap;
	char* arr;
	
public:
	DynArr();
	DynArr(size_t);
	~DynArr();
	DynArr(const DynArr&);
	DynArr& operator = (const DynArr&);


public:
	size_t getSize() const;
	size_t getCap() const;

private:
	void copy(const DynArr&);
	void clear();

	void incSize();
	void decSize();
	void moveForward(size_t, int);
	T* newArr(size_t) const;

public:
	T& operator [] (const size_t);

public:
	bool isEmpty() const;
	void putIn(T&, size_t = size);
	T& takeOut(size_t = 0);

	void print() const;

};

// default c-tor
template <typename T>
DynArr<T>::DynArr()
{
	cap = 1;
	arr = new T[cap];
	size = 0;
}

// c-tor with the capacity
template <typename T>
DynArr<T>::DynArr(size_t cap)
{
	this->cap = cap;
	arr = new T[cap];
	size = 0;
}

// d-tor
template <typename T>
DynArr<T>::~DynArr()
{
	clear();
}

// copy c-tor
template <typename T>
DynArr<T>::DynArr(const DynArr& d)
{
	copy(d);
}

template <typename T>
DynArr<T>& DynArr<T>::operator = (const DynArr& d)
{
	if (this != &d)
	{
		clear();
		copy(d);
	}
	return *this;
}

// gets
template <typename T>
size_t DynArr<T>::getSize() const
{
	return size;
}

template <typename T>
size_t DynArr<T>::getCap() const
{
	return cap;
}

// Help Functions
// copy
template <typename T>
void DynArr<T>::copy(const DynArr& d)
{
	cap = d.cap;
	size = d.size;
	arr = new T[cap];
	for (size_t i = 0; i < size; ++i)
		arr[i] = d.arr[i];
}

// copy for pointers
template <typename T>
void DynArr<T*>::copy(const DynArr& d)
{
	cap = d.cap;
	size = d.size;
	arr = new T*[cap];
	for (size_t i = 0; i < size; ++i)
		*arr[i] = *d.arr[i];
}

// copy for char pointers
template<>
void DynArr<char*>::copy(const DynArr& d)
{
	cap = d.cap;
	size = d.size;
	arr = new char*[cap];
	for (size_t row = 0; row < size; ++row)
	{
		arr[row] = new char[strlen(d.arr[row]) + 1];
		strcpy(arr[row], d.arr[row]);
	}
}

template <typename T>
void DynArr<T>::clear()
{
	delete[] arr;
}

template <typename T>
void DynArr<T*>::clear()
{
	for (size_t i = 0; i < cap; ++i)
		delete[] arr[i];
	delete[] arr;
}


template <typename T>
T* DynArr<T>::newArr(size_t cap) const
{
	T* newArr = new T[cap];
	for (size_t i = 0; i < size; ++i)
		newArr[i] = arr[i];
	return newArr;
}

// newarr for pointers
template <typename T>
T** DynArr<T*>::newArr(size_t cap) const
{
	T** newArr = new T*[cap];
	for (size_t i = 0; i < size; ++i)
		*newArr[i] = *arr[i];
	return newArr;
}

// newarr for char*
template <typename T>
char** DynArr<char*>::newArr(size_t cap) const
{
	char** newArr = new char*[cap];
	for (size_t i = 0; i < size; ++i)
	{
		newArr[i] = new char*[strlen(arr[i]) + 1];
		strcpy(newArr[i], arr[i]);
	}
	return newArr;
}

template <typename T>
void DynArr<T>::incSize()
{
	assert(size == cap);
	cap *= 2;
	arr = newArr(cap);
}

template <typename T>
void DynArr<T>::decSize()
{
	assert(size < cap / 2);
	cap /= 2;
	arr = newArr(cap);
}

template <typename T>
void DynArr<T>::moveForward(size_t pos, int times)
{
	times *= -1;
	assert(times != 0);
	if (size + times >= cap)
		incSize();

	if (times < 0)
		for (int i = pos; i < size; ++i)
			arr[i + times] = arr[i];
	else if (times > 0)
		for (int i = size; i >= pos; ++i)
			arr[i + times] = arr[i];

	size += times;
	if (size + times < cap / 2)
		decSize();
}

// Array Manipulation
template <typename T>
T& DynArr<T>::operator [](const size_t pos)
{
	if (pos >= size)
		throw std::out_of_range("The position must be a non-negative intiger!");
	return arr[pos];
}

template <typename T>
bool DynArr<T>::isEmpty() const
{
	return size == 0 ? true : false;
}

template <typename T>
void DynArr<T>::putIn(T& elem, size_t pos = size)
{
	assert(size <= cap);
	if (pos > size)
		throw; // invalid argument
	else if (pos < size)
		moveForward(pos, 1);
	arr[pos] = elem;
}

template <>
void DynArr<Command*>::putIn(Command*& elem, const size_t pos)
{
	assert(size <= cap);
	if (pos > size)
		throw; // invalid argument
	else if (pos < size)
		moveForward(pos, 1);
	*arr[pos] = *elem;
}



template <typename T>
T& DynArr<T>::takeOut(size_t pos = 0)
{
	assert (size == 0);
	T elem = arr[pos];

	moveForward(pos + 1, -1);
	return elem;
}

template <>
Command*& DynArr<Command*>::takeOut(size_t pos)
{
	assert(size == 0);
	Command* elem = arr[pos];

	moveForward(pos + 1, -1);
	return elem;
}