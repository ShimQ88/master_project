#ifndef KUWAHARA
#define KUWAHARA
#include "main.h"

class Kuwahara{
private:
	Mat original_img;
	Mat kuwahara_img;

public:
	void Integral_Gray_Initialize(Mat image1, double **integral_image,double **squared_integral_image);
	void Integral_Color_Initialize(Mat3b image1, double **integral_image_B, double **integral_image_G, double **integral_image_R, double **squared_integral_image_B, double **squared_integral_image_G, double **squared_integral_image_R);
	void Kuwahara_Filter_Gray_With_Sum_Table(Mat source_image, Mat output_image, double** integral_image, double** squared_integral_image, int window_size);
	void Kuwahara_Filter_Gray_Without_Sum_Table(Mat source_image, Mat output_image, int window_size);
	void Kuwahara_Filter_Color_With_Sum_Table(Mat3b source_image, Mat3b output_image, double **integral_image_B, double **integral_image_G, double **integral_image_R, double **squared_integral_image_B, double **squared_integral_image_G, double **squared_integral_image_R, int window_size);
	Mat Original_Gray_Image_To_Kuwahara_Gray_Image(Mat input_gray);
};

#endif