#pragma once
#include "std_lib_facilities.h"
#include "ImageIOBase.h"
class MhdImageIO : public ImageIOBase
{
public:
	MhdImageIO(const string& file_name) :ImageIOBase(file_name) {};
	Image read() const override;
	void write(const Image& image) const override;
protected:
};

//.............................................................................//
Image MhdImageIO::read() const {

	//-------- open the header .mhd ---------// 

	// initializing header parameters based on exercise 02.1
	int NDims=0;
	string ElementType = "";
	Image::dimension DimSize = {};
	string ElementDataFile = "";
	int DimCounter = 0;

	ifstream ifs{ filename };
	if (!ifs) error("Can't open header\n", filename);
for(string s; ifs>>s;)
 {

		if (s == "NDims")
		{
			ifs >> s;
			ifs >> s;
			NDims = stoi(s);
		}
		if (s == "DimSize")
		{
			ifs >> s;
			while (ifs >> s && s != "ElementType")
			{
				DimSize[DimCounter] = stoi(s); //convert string numbers to integers to represent dimensions
				DimCounter++;
				
			}
		}
		if (s == "ElementType")
		{
			ifs >> s;
			ifs >> s;
			ElementType = s;
		}
		if (s == "ElementDataFile")
		{
			ifs >> s;
			ifs >> s;
			ElementDataFile = s;
		}
		
	}
	ifs.close();
	DimSize[3] = { 1 };
	DimSize[4] = { 1 }; 

	// ------- ERROR MANAGEMENT ------//

	if (NDims < 1 || NDims>5)
	{
		error("Number of dimensions has to be between 1 and 5\n");
	}
	if (NDims != DimCounter)
	{
		error("Number of dimensions mentioned doesn't match the actual size of the image\n ");
	}
	if (ElementType != "MET_SHORT")
	{
		error("Element type should be MET_SHORT, not ", ElementType);
	}
	for (int i = 0; i <= 4; i++)
	{
		if (DimSize[i] < 1)
			error("The size of each dimension must be at least 1\n");
	}
	

	ifstream rawfs{ ElementDataFile, ios_base::binary }; // open data file as binary
	if (!rawfs)
	{
		if (rawfs.bad())
		{
			rawfs.close();
			error("Reading of file has failed ", ElementDataFile);
		}
		if (rawfs.fail())
		{
			rawfs.close();
			error("Reading of file has failed ", ElementDataFile);
		}
		error("Can't open the .raw file to read.\n", ElementDataFile);   //checks if the files opened correctly
	}
	

	Image mhdimage(DimSize);

	int counter = 0;
	int img_size = DimSize[0] * DimSize[1] * DimSize[2] * DimSize[3] * DimSize[4]; 

	for (Image::iterator p = mhdimage.begin(); p != mhdimage.end(); ++p)
	{   // data stored in type short
		if (rawfs.eof()) error("less data included than expected");
		rawfs.read(as_bytes(*p), sizeof(Image::value_type));
		counter++;
	}
	if (counter != img_size)
		error("Header dimensions incompatible with image size \n");
	// find if we have a wrong data type in the file
	rawfs.seekg(0, ios::end); 
	int file_size = rawfs.tellg(); //defining the size of our mhdimage file
	if (file_size != sizeof(short) * counter) error("Wrong data type. Only include files with data type='short' \n"); //compare the file of the mhdimage with the file that it should have if it is a proper MhdImage file
	rawfs.close();
	return mhdimage;
}


void MhdImageIO::write(const Image& image) const
{

	//-------- open the header .mhd and write ---------// 

	ofstream ofs{ filename };		//open mhd file as text to copy informations
	if (!ofs)
		error("Can't open the header .mhd to write.\n", filename);

	int NDims=0;
	int DimSize[N_DIM];
	string ElementType = "MET_SHORT";
	string ElementDataFile = filename;
	ElementDataFile.resize(ElementDataFile.size() - 3); //changing ElementDataFile from .mhd to .raw
	ElementDataFile = ElementDataFile + "raw";
	Image::dimension dimensions = image.size(); 

	for (int i = 0; i < N_DIM && dimensions[i] != 1; ++i)
	{  //copy dimension from dimensions to array DimSize
		DimSize[i] = dimensions[i];
		NDims = i + 1;
	}
	ofs << "ObjectType = Image\n";
	ofs << "NDims = " << NDims << "\n";
	ofs << "BinaryData = True\n";
	ofs << "BinaryDataByteOrderMSB = False\n";
	ofs << "CompressedData = False \n";
	ofs << "DimSize =";
	for (int i = 0; i < NDims; ++i)
		ofs << " " << DimSize[i];
	ofs << "\nElementType = " << ElementType << " \n";
	ofs << "ElementDataFile = " << ElementDataFile;

	ofs.close();


	ofstream rofs{ ElementDataFile, ios_base::binary };	 // open the raw file as binary
	if (!rofs)
		error("Can't open the output file\n", filename);	//checks if the files opened correctly
	for (Image::const_iterator p = image.begin(); p != image.end(); ++p) {   // data stored in type short
		rofs.write(as_bytes(*p), sizeof(Image::value_type));
	}
	
}






