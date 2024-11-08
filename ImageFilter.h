#pragma once
#include "std_lib_facilities.h"
#include "ImageIOFactory.h"


class ImageFilter
{
    typedef short T; // all we do is short 
public:
    // Constructor. 
    ImageFilter() {};
    // Destructor. This class does not own any free store allocated objects; 
    //but derived classes might, so it's virtual and empty 
    virtual ~ImageFilter() {};
    // Because derived classes can add data/function/type members, we want to  avoid copying (and thus slicing) 
    ImageFilter(const ImageFilter&) = delete;
    ImageFilter& operator=(const ImageFilter&) = delete;
    // Set the input image for this filter 
    void setInput(const Image& i) {
        // The input should be const. However, we cannot store it that way in our 
        //  base class. Therefore we cast away the const and store a pointer to the 
        // original data. Upon using the input data in update(), we cast it back. 
        _input = const_cast<Image*>(&i);
    }
    // Get the input back as a const ref 
    const Image& getInput() const {
        // Didn't touch _input, cast it back to const; 
        return const_cast<const Image&>(*_input);
    }
    // Get the output image result of this filter; available after calling update() 
    Image getOutput() const { return _output; };
    // Update the image filter and compute the output 
    virtual void update()
    {
        // Didn't touch _input, cast it back to const; then execute() 
        execute(getInput());
    }
protected:
    // Container for the output image 
    Image _output{{1,1,1,1,1}};
    // This method should be overloaded in your derived class and implement the image filter that fills _output 
    virtual void execute(const Image& i) = 0;
private:
    // Temporary storage of a pointer to the image data. Because we do some const-magic, we keep it private. 
    Image* _input;
};


