#ifndef MAIN
#define MAIN

// System Headers
#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <chrono>
#include <ctime>
#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <glob.h>


// Opencv Headers
// #include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/ml/ml.hpp"


// //THis is for tesing
#include <sys/stat.h>//This is using for mkdir
// #include <sys/types.h> 
// #include <sys/stat.h>
// #include <unistd.h>
// #include <stdlib.h>


//namespace
using namespace std;
using namespace cv;
using namespace cv::ml;
using namespace std::chrono;

//Headers from include
#include "contour_yolo_creater.h"
#include "the_second_filtering.h"
#include "the_final_filtering.h"
#include "directory_creater.h"
#include "create_weight.h"
#include "machine_learning_data_preparation.h"
#include "the_second_cycling.h"
#include "detect.h"
// #include "ml_class.h"

//pixel load
#define Mpixel(image,x,y) ((uchar *)((image).data +(y)*((image).step)))[(x)*((image).channels())]//gray color space
#define pixelB(image,x,y) image.data[image.step[0]*y+image.step[1]*x]	//Blue color space
#define pixelG(image,x,y) image.data[image.step[0]*y+image.step[1]*x+1]	//Green color space
#define pixelR(image,x,y) image.data[image.step[0]*y+image.step[1]*x+2]	//Red color space

#define the_number_of_file_in_folder 100


//functions
void info();
string Read_Index_From_File(const string& path_index_file);
int function2(string& paths_dataset, string *out_data);
int main(int argc, char *argv[]);

#endif