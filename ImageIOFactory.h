#pragma once
#include "std_lib_facilities.h"
#include "PipImageIO.h"
#include "MhdImageIO.h"
class ImageIOFactory {
public: 
	static unique_ptr<ImageIOBase> getIO(const string& filename); //ImageIOFactory only function. Note that we improved the old version of
	                                                              //getIO by returning a unique_ptr. This way, the user doesn't have to delete the
	                                                              //io pointer in the source.cpp each time.
};
unique_ptr<ImageIOBase> ImageIOFactory::getIO(const string& filename)
{
	string tp = filename.substr(filename.length() - 3); //finding the extension of the input file
	if (tp == "pip")
	{
		unique_ptr<ImageIOBase> f{ new PipImageIO(filename) }; //creating a unique_ptr of type ImageIOBase to point in PipImageIO object
		return f;
	}
	else if (tp == "mhd")
	{
		unique_ptr<ImageIOBase> f{ new MhdImageIO(filename) }; //same but for MhdImageIO object this time
		return f;
	}	
}
