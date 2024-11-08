#pragma once
#include "ImageFilter.h"
class MaskFilter : public ImageFilter
{
    typedef short T;

public:
    // Constructor that initializes the mask at 0
    MaskFilter() : _m({ 0 }) {}

    // Get and set functions for the mask
    Image getInputMask() const { return _m; }
    void setInputMask(Image mask) { _m = mask; }

protected:
    // Override the execute function
    virtual void execute(const Image& i) override
    {
        // Resizing the output

        _output.resize(i.size());

        // Bring the mask parameter within scope, so it can be given to
        // the lambda initializer
        const auto m = _m;

        // Apply the mask
        for (unsigned int t = 0; t < i.dim(4); ++t) {
            for (unsigned int c = 0; c < i.dim(3); ++c) {
                for (unsigned int z = 0; z < i.dim(2); ++z) {
                    for (unsigned int y = 0; y < i.dim(1); ++y) {
                        for (unsigned int x = 0; x < i.dim(0); ++x) {
                            _output(x, y, z, c, t) = i(x, y, z, c, t) * m(x, y, z, c, t); //applying the mask filter
                        }
                    }
                }
            }
        }
    }

private:
    // Declaing the mask
    Image _m;
};
