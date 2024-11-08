#pragma once
#include "ImageFilter.h"
class ConvolutionFilter : public ImageFilter {

    typedef short T;

public:
    //Constructor of this convolution filter class. Initialising the kernel to 0. The same way as in Threshold class.
    ConvolutionFilter() : _k{ 0 } {}


    // Get and set functions for the kernel
    int getKernel() const { return _k; }
    void setKernel(int kernel) { _k = kernel; }

protected:
    // Override the execute function
    virtual void execute(const Image& i) override
    {
        //resize function for the output

        _output.resize(i.size());

        // Bring the kernel parameter within scope, so it can be given to
        // the lambda initializer
        const auto k = _k;

        // Nested for loops for the kernel. Applying each one to the dimensions of the image
        for (unsigned int t = 0; t < i.dim(4); ++t) {
            for (unsigned int c = 0; c < i.dim(3); ++c) {
                for (unsigned int z = 0; z < i.dim(2); ++z) {
                    for (unsigned int y = 0; y < i.dim(1); ++y) {
                        for (unsigned int x = 0; x < i.dim(0); ++x) {
                            if (x <= k || y <= k)
                                _output(x, y, z, c, t) = i(x, y, z, c, t);
                            else {
                                T g = 0;
                                for (unsigned int kx = 0; kx < k; ++kx) {
                                    for (unsigned int ky = 0; ky < k; ++ky) {
                                        g = g + i(x + kx - (k / 2), y + ky - (k / 2), z, c, t);
                                    }
                                }
                                _output(x, y, z, c, t) = g * 1 / pow(k, 2);            
                            }
                        }
                    }
                }
            }
        }
    }

private:
    // Declaring the kernel 
    int _k;
};
