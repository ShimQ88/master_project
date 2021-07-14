#ifndef THE_SECOND_CYCLING
#define THE_SECOND_CYCLING
#include "main.h"
struct YOLO_TXT_INFO{
	string class_numb;
	float accuracy;
	Point p1;
	Point p2;
};

void Main_Contour_Yolo_Creater_With_Window_Info(const string class_number, const string path_dataset, const string idx_value, const string path_contour_file,const string path_yolov5, const string save_location);
void Main_Contour_Yolo_Creater_With_Window_Info_Video(const string class_number, const string path_dataset, const string path_txt, const string idx_value, const string path_contour_file, const string path_yolov5, const string save_location);

#endif