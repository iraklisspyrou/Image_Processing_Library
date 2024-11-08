#pragma warning(default:4996)
#include "ImageFilter.h"
#include "ThresholdFilter.h"
#include "StatisticsFilter.h"
#include "ConvolutionFilter.h"
#include "MaskFilter.h"
#include "NegativeFilter.h"
#include "GammaFilter.h"
#define N_DIM 5
int main()
{
		try {
			string filename;
			cout << "Please enter a PIP or MHD filename: " << endl;
			cin >> filename;
			//error handling if the file is not an .mhd and .pip
			string e = filename.substr(filename.length() - 3);    //reading the last three strings of the file
			while (e != "pip" && e != "mhd")
			{
				cout << "Please input the right file type: \n";
				cin >> filename;
				e = filename.substr(filename.length() - 3);   //inout of the new file and checking again if it is a .pip or .mhd
			}

			//error handling if the file is in the diractory
			ifstream ifs;
			ifs.open(filename);
			while (!ifs)
			{
				cout << "The file doesn't exit. Please input another file: " << endl;
				cin >> filename;
				ifs.open(filename);

			}
			unique_ptr<ImageIOBase> io = ImageIOFactory::getIO(filename);
			auto image = io->read();
			io = ImageIOFactory::getIO("brain_out.pip");
			io->write(image);


		//-----------------The Statistics Filter-----------------------------

			StatisticsFilter s;
			s.setInput(image);
			s.update();
			short min = s.getMin();
			cout << "Min Pixel Value : " << min << '\n'; //printing the min value of the image
			short max = s.getMax();
			cout << "Max Pixel Value : " << max << '\n'; //printing the max value of the image
			short mean = s.getMean();
			cout << "Mean of Pixel Vales: " << mean << '\n'; //printing the mean value

			//----------------Executing the Threshold Filter--------------------

			ThresholdImageFilter tf;

			tf.setInput(image);
			tf.setThreshold(90);  //enter the value of the threshold manually
			tf.update();
			auto image_thres = tf.getOutput();
			io = ImageIOFactory::getIO("brain_out_thres.pip");
			io->write(image_thres);



			//--------------The Convolution Filter------------------------------

			ConvolutionFilter cf;
			cf.setInput(image);
			cf.setKernel(4);
			cf.update();
			auto image_conv_fil = cf.getOutput();
			io = ImageIOFactory::getIO("brain_out_conv.pip");
			io->write(image_conv_fil);




		   //---------------------The Mask filter-------------------------------------------

			Image mm(image_thres); //getting the threshold in order to apply the mask

			MaskFilter mask_f;
			mask_f.setInput(image);
			mask_f.setInputMask(mm);
			mask_f.update();
			auto image_m = mask_f.getOutput();
			io = ImageIOFactory::getIO("brain_out_mask.pip");
			io->write(image_m);

			//-------------------The Negative Filter----------------------------------------

			NegativeFilter neg_f;
			neg_f.setInput(image);
			neg_f.update();
			auto image_neg = neg_f.getOutput();
			io = ImageIOFactory::getIO("brain_out_neg.pip");
			io->write(image_neg);

			//------------------ The Gamma filter ----------------------------------------

			GammaFilter gamma;
			gamma.setInput(image);
			gamma.setGamma(1);
			gamma.update();
			auto image_gamma = gamma.getOutput();
			io = ImageIOFactory::getIO("brain_out_gamma.pip");
			io->write(image_gamma);


		}

		catch (exception& e) {
			cerr << "Exception: " << e.what() << '\n';
			keep_window_open();
			return 1;
		}
		catch (...) {
			cerr << "Unknown exception\n";
			keep_window_open();
			return 2;
		}
}
