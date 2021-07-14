#ifndef THE_SECOND_FILTERING
#define THE_SECOND_FILTERING
#include "main.h"

void Main_The_Second_Filtering(const string path_dataset,const string idx_value); 

class The_Second_Filtering{
private:
    glob_t glob_contour, glob_yolo;
    string path_images;
    string idx_numb;
    int idx;
    int the_number_of_files;
    char *final_path_contour;
    char *final_path_yolo;
    char *parent_path_contour;
    char *parent_path_ori_img;
    Mat *image_contour;
    Mat *image_original;
    string *file_read;

    string out_path_contour;
    string out_path_yolo;
    string out_path_parent;





    // string folder_name_root;
    // string folder_name_contour;
    // string folder_name_yolo;
    Mat merged_img;
    string selected_idx; 
public:
    The_Second_Filtering(string i_folder_name_root, string i_idx_numb, string i_out_path_contour, string i_out_path_yolo, string i_out_path_parent );
    ~The_Second_Filtering();
    void Main_Process();
    string Decision_Part();
    bool Digit_Check(string input_digit);
    bool Determine_Final_Path_Contour(string i_path_images, string i_idx_numb);
    bool Path_Check();
    int Determine_Folder_Numb(string i_idx_numb);
    void Splite_Data_Yolo_and_Contour_v3(string resource_file, string* yolo_data, string* contour_data, string* img_name );
    void Read_Info_Contour_Yolo();
    void Save_Info_of_Selected_Imgs();
    void Save_Yolo(string img_name, string yolo_data);
    bool Name_Check(string temp_ori_name, string name_ori_img);
    bool Load_Img_and_Store_to_Mat();
    bool Load_Img_and_Store_to_Mat_V2();
    void Merge_Multi_Mats_to_Single_Mat_v2(Mat resource_mat[], int the_number_of_files);
};


#endif