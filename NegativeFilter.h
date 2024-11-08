#pragma once
#include "ImageFilter.h"
class NegativeFilter : public ImageFilter
{
    typedef short T;

public:
    // Constructor of the negative class
    NegativeFilter() {}


protected:
    // Override the execute function
    virtual void execute(const Image& i) override
    {
        // Resize the output
        _output.resize(i.size());

        // Apply the negative value
        for (unsigned int t = 0; t < i.dim(4); ++t) {
            for (unsigned int c = 0; c < i.dim(3); ++c) {
                for (unsigned int z = 0; z < i.dim(2); ++z) {
                    for (unsigned int y = 0; y < i.dim(1); ++y) {
                        for (unsigned int x = 0; x < i.dim(0); ++x) {
                            _output(x, y, z, c, t) = i(x, y, z, c, t) * -1;   //changing the image values to their negative components
                        }
                    }
                }
            }
        }
    }

};
