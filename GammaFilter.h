#pragma once
#include "ImageFilter.h"
class GammaFilter : public ImageFilter {

	typedef short T;

public:

	GammaFilter() : _g(1) {} //constructor initialising the default value to 1

	T getGamma() { return _g; }
	void setGamma(T setGamma) { _g = setGamma; }


protected:
	// Override the execute function 
	virtual void execute(const Image& i) override
	{
		// Clear and resize the output 
		_output.resize(i.size());

		// Bring the parameter within scope, so it can be given to 
		// the lambda initializer 
		const auto g = _g;
		double ShortMax = 32767.0; //max value of a short type data


		transform(begin(i), end(i), begin(_output),
			[g, ShortMax](T value) { return short(pow(double(value / ShortMax), (1.0 / g)) * ShortMax); });

	}



private:

	T _g;
};
