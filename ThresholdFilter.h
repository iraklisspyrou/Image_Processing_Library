#pragma once
#include "ImageFilter.h"
class ThresholdImageFilter : public ImageFilter
{
    typedef short T;
public:
    // Constructor that initializes the threshold at 0 
    ThresholdImageFilter() : _t(0) {}
    // Get and set functions for the threshold value 
    T getThreshold() const { return _t; }
    void setThreshold(T threshold) { _t = threshold; }
protected:
    // Override the execute function 
    virtual void execute(const Image& i) override
    {
        // Clear and resize the output 
        _output.resize(i.size());
        // Bring the threshold parameter within scope, so it can be given to 
        // the lambda initializer 
        const auto t = _t;
        // Do the thresholding with an std::transform, see Chapter 21 
        transform(i.begin(), i.end(), _output.begin(),
            [t](T value) { return value > t ? T(1) : T(0); }
        );
    }
private:
    // The threshold 
    T _t;
};
