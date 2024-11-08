#pragma once
#include "std_lib_facilities.h"
#include "Image.h"
#define N_DIM 5

class ImageIOBase {
public:
	virtual ~ImageIOBase(){}
	virtual Image read() const = 0;
	virtual void write(const Image&) const = 0;
	ImageIOBase(const ImageIOBase&) = delete; // no copy construct
	ImageIOBase& operator=(const ImageIOBase&) = delete; // no copy assign
protected:
	string filename;
	ImageIOBase(string file_name) : filename{ file_name } {}; // access only by derived members
};


