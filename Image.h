#pragma once
#include "std_lib_facilities.h"
#define N_DIM 5
class Image
{
 public:
	// Internally used data types 
	typedef short T;
	typedef std::array<int, N_DIM> dimension;

	// STL container and iterator typedefs 
	typedef T value_type;
	typedef T* iterator;
	typedef const T* const_iterator;
	typedef T& reference;
	typedef const T& const_reference;

	// Constructors and destructor 
	Image(dimension);
	Image(const Image&);
	Image(Image&&);

	virtual ~Image() {
		delete[] _data;
	}

	// Assignment operators 
	Image& operator=(const Image&);
	Image& operator=(Image&&);

	// Basic iterators, this generates a C4996 error in Visual Studio. 
	// Disable that in main.cpp with: #pragma warning(default:4996) 
	iterator begin() {
		return _data;
	}
	iterator end() {
		int r = _dimensions[0] * _dimensions[1] * _dimensions[2];
		return _data + r;
	}
	const_iterator begin() const {
		return _data;
	}
	const_iterator end() const {
		int r = _dimensions[0] * _dimensions[1] * _dimensions[2];
		return _data + r;
	}

	// Size and resize 
	dimension size() const {    // the dimension object 
		return _dimensions;
	}         
	unsigned int dim(unsigned int d) const; // get the size of a dimension 
	unsigned int nr_dims() const;           // number of dimensions > 1 
	void resize(const dimension& d);        // resize this image 

	// Pixel value lookup, should support out-of-image coordinates by clamping to 0..dim 
	value_type operator()(int x=0, int y = 0, int z = 0, int c = 0, int t = 0) const {
		return _data[(t * _dimensions[3] * _dimensions[2] * _dimensions[1] * _dimensions[0]) + (c * _dimensions[2] * _dimensions[1] * _dimensions[0]) + (z * _dimensions[1] * _dimensions[0]) + (y * _dimensions[0]) + x];
	}
	reference operator()(int x = 0, int y = 0, int z = 0, int c = 0, int t = 0) {
		return _data[(t * _dimensions[3] * _dimensions[2] * _dimensions[1] * _dimensions[0]) + (c * _dimensions[2] * _dimensions[1] * _dimensions[0]) + (z * _dimensions[1] * _dimensions[0]) + (y * _dimensions[0]) + x];
	}
private:
	dimension _dimensions;
	T* _data;
};

Image::Image(dimension d) : _dimensions{ d }, _data{ new T[d[0] * d[1] * d[2]*d[3]*d[4]]} {} //defining constructor
Image::Image(const Image& i) : _data(new T[i._dimensions[0] * i._dimensions[1] * i._dimensions[2]*_dimensions[3]*_dimensions[4]]), _dimensions{i._dimensions} { //defining copy constructor
	for (int i = 0; i < N_DIM; ++i) {
		if (_dimensions[i] == 0)_dimensions[i] = 1;
		if (_dimensions[i] < 0) error("\n\t Error in dimension \n");
	}
	int dim = _dimensions[0] * _dimensions[1] * _dimensions[2]*_dimensions[3]*_dimensions[4];
	for (int j = 0; j < dim; ++j) {
		_data[j] = i._data[j]; //copy all elements of image i to a new image object
	}
}
Image::Image(Image&& i) : _dimensions{ i._dimensions }, _data{ i._data } { //defining move constructor
	i._dimensions = { 0,0,0,0,0 };
	i._data = nullptr;
}

Image& Image::operator=(const Image& i) { //defining assignment operation
	int r = i._dimensions[0] * i._dimensions[1] * i._dimensions[2] ;
	T* p = new T[r]; //creating a new pointer with occupied memory equal to the number of pixels
	for (int k = 0; k < r; ++k) p[k] = i._data[k]; 
	delete[] _data;
	_dimensions = i._dimensions; //copy all the elements
	_data = p;
	return *this;
}
Image& Image::operator=(Image&& i) {
	delete[] _data;
	_data = i._data;
	_dimensions = i._dimensions;
	i._data = nullptr;
	i._dimensions = { 0,0,0,0,0 };
	return *this;
}
unsigned int Image::dim(unsigned int d) const { //definig dim function. We return the value of each dimension
	if (d == 0) 
		return _dimensions[0];
	else if (d == 1)
		return _dimensions[1];
	else if (d == 2)
		return _dimensions[2];
	else if (d == 3)
		return _dimensions[3];
	else if (d == 4)
		return _dimensions[4];

	else
	{
		error("non existing dimension");
		return 0;
	}
}
unsigned int Image::nr_dims() const { 
	int count = 0;
	for (int i = 0; i <= 4; ++i) {
		if (_dimensions[i] > 1) ++count;         
	}
	return ++count;  
}   
void Image::resize(const dimension& d) { //defining resize function. We resize the old image with the size of a new image 
	int oldsize = _dimensions[0] * _dimensions[1] * _dimensions[2]*_dimensions[3]*_dimensions[4];
	int newsize = d[0] * d[1] * d[2]*d[3]*d[4];
	if (newsize > oldsize)
	{
		T* p = new T[newsize];
		delete[] _data;
		_data = p;
		for (int i = 0; i < newsize; ++i) _data[i] = 0;
		oldsize = newsize;
		_dimensions = d;
	}

	
}
