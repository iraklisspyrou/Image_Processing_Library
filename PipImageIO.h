#pragma once
#include "ImageIOBase.h"
class PipImageIO : public ImageIOBase
{
public:
	PipImageIO(const string& filename) :ImageIOBase(filename) {};
	Image read() const override;
	void write(const Image& image) const override;
};

Image PipImageIO::read() const
{
	ifstream ifs{ filename, ios_base::binary }; // open file as binary
	if (!ifs)
	{
		if (ifs.bad() || ifs.fail()) // checking if the file opens correctly
		{
			ifs.close();
			error("Can't read file", filename);
		}
	}
	array<int, N_DIM> dimensions = {};
	// read header
	unsigned char x;
	ifs.read(as_bytes(x), sizeof(unsigned char));
	for (int i = 0; i <= 4; ++i)
	{
		ifs.read(as_bytes(dimensions[i]), sizeof(int));
	}
	if (dimensions[0] < 1 || dimensions[1] < 1 || dimensions[2] < 1 || dimensions[3] < 1 || dimensions[4] < 1) //checking dimensions
	{
		error("PIP Dimension error. \n");
	}

	// read pixel values
	Image pipimage(dimensions);
	int counter = 0;
	int img_size = dimensions[0] * dimensions[1] * dimensions[2] * dimensions[3] * dimensions[4];
	for (Image::iterator p = pipimage.begin(); p != pipimage.end(); ++p) 
	{
		ifs.read(as_bytes(*p), sizeof(Image::value_type));
		counter++;
	}
	if(counter!=img_size)     //find if we have less data than we should                                     
		error("Number of data doesn't match the number of the data provided from the header.\n");
	//find if we have a wrong data type in the file
	ifs.seekg(0, ios::end);  
	int file_size = ifs.tellg(); //defining the size of our pipimage file
	if (file_size != sizeof(unsigned char)+ sizeof(int)*5+sizeof(Image::value_type) * counter) error("Wrong data type. Only include files with data type='short' \n"); //compare the file of the pipimage with the file that it should have if it is a proper PipImage file
	return pipimage; 
	ifs.close();
}
void PipImageIO::write(const Image& image) const
{
	ofstream ofs{ filename, ios_base::binary };
	if (!ofs) error("can't open output file", filename);
	unsigned char x = 2;
	//data type 2=short
	ofs.write(as_bytes(x), sizeof(unsigned char));
	//write dimensions
	for (int x : image.size())
		ofs.write(as_bytes(x), sizeof(int));
	//write image data
	for (Image::const_iterator p = image.begin(); p != image.end(); ++p)
		ofs.write(as_bytes(*p), sizeof(Image::value_type));
}