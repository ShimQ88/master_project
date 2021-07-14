#ifndef CONTOUR_YOLO_CREATER
#define CONTOUR_YOLO_CREATER
#include "main.h"
#include "kuwahara.h"
// #include "directory_creater.h"
#define PI 3.1415926535897932384626433832795029L
#define n_CE 21

struct YOLO_TXT_INFO;

struct Yolo{
	float yolo_height;
    float yolo_width;
    float yolo_x;
    float yolo_y;
};


class Fourier_Descriptors{
public:
	int type;
	/*type detail
    -1=critical error(fail target load)
    0=fail next and prev file load
    1=success prev load
    2=success next load
    3=success next and prev load
    4=else from load
    */
	string class_number="0";//default value
	string target_file_name;

	// string prev_file_name;
	// string next_file_name;

	//original images
	Mat target_color_img;
    Mat target_gray_img;
    Mat target_ROI;

    //load next and previous gray images
    Mat next_gray_img;
    Mat prev_gray_img;

    //kuwahara images;
    Mat next_ku_img;
    Mat prev_ku_img;
    Mat target_ku_img;

    //subtracted images
    Mat subtracted_gray_img;
    Mat subtracted_gray_img2;
    // Mat ROI_img;

    Mat thresholded_img;
    Mat ROI_contour;

    // Mat ROI_img;
    string contour_txt;
    Mat final_CE_data;
    Point label_location;
    float label_location_width;
    float label_location_height;
    Yolo YL;

	int Load_Imgs(string p_target_file_name, string p_prev_file_name, string p_next_file_name);
	int Load_Imgs_Video(string p_target_file_name, string p_prev_file_name, string p_next_file_name);
    bool Main_The_First_Process(string p_target_file_name, string p_prev_file_name, string p_next_file_name);
	// Fourier_Descriptors();
	// Fourier_Descriptors(string p_target_file_name, string p_prev_file_name, string p_next_file_name);
	void EllipticFourierDescriptors(std::vector<Point>& contour, std::vector<float> &CE);
	int Find_The_Object_Contour(std::vector<vector<Point>>contours,Point center_of_object);
	int Distance_Between_Points(Point p1, Point p2);
	int Determine_Frame_Size(Mat input, Point obj_center, int frame_size, int append_size );
	Point Draw_Rect_Box(Mat input_image, Point* p1, Point* p2, int loop_number);
    Point Draw_Rect_Box2(Mat input_image, Point* p1, Point* p2, int loop_number);
	int Blob(Mat image2, Mat image3,Point *p1_rec, Point *p2_rec);
	Mat Cropping_ROI(Mat imput_image,Point center_of_object, int kernel_size);
    bool Main_The_Second_Cycle_Process(string p_target_file_name, string p_prev_file_name, string p_next_file_name, YOLO_TXT_INFO yolo_info);
    bool Main_The_Video_Cycle_Process(string p_target_file_name, string p_prev_file_name, string p_next_file_name, YOLO_TXT_INFO yolo_info);
    Mat Main_Detection_V1(Mat back_ground_img, Mat current_img);
    Mat Main_Detection_V2(Mat back_ground_img, Mat current_img);
};


glob_t Read_Dataset_Dir(string temp_path);
void Main_Contour_Yolo_Creater(const string class_number,const string path_dataset,const string idx_value);
string Extract_File_name_From_Full_Path(string name);
bool Check_Numb_In_A_Row(string prev_name, string target_name);
bool Index_Saver(string path,unsigned int i);
#endif