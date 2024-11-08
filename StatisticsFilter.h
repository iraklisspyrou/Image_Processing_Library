#pragma once
#pragma once

#include "ImageFilter.h"
#include "std_lib_facilities.h"
#include "Image.h"


//--------------------------------------------Statistics Filter-------------------------------------------------------

class StatisticsFilter : public ImageFilter {

    typedef short T;

public:
    StatisticsFilter() {}

    T getMin() { return min; }
    T getMax() { return max; }
    double getMean() { return mean; }

protected:

    virtual void execute(const Image& i) override  //execute function
    {

        _output.resize(i.size());  //clear and resize output
        min = *min_element(i.begin(), i.end());  //min function
        max = *max_element(i.begin(), i.end());   //max function
        Image::dimension dimensions = i.size();
        int image_size = dimensions[0] * dimensions[1] * dimensions[2];
        double w = dimensions[0];
        for (Image::const_iterator p = i.begin(); p != i.end(); ++p)
        {
            w = w + *p;
        }
        mean = w / image_size;



    }

private:
    short min;
    short max;
    double mean;

};


