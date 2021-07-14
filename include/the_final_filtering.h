#ifndef THE_FINAL_FILTERING
#define THE_FINAL_FILTERING
#include "main.h"

void Main_The_Final_Filtering(const string path_dataset,const string idx_value);

class The_LAST_FILTERING{
private:
    glob_t glob_contour, glob_yolo;

    string path_images;
    string out_path_contour;
    string out_path_yolo;
    string out_path_parent;

    int idx;
    int the_number_of_selected_idx;
    int *the_list_of_selected_idx;
    Mat *img_contour;
    Mat *img_yolo;
    string *file_info;
    string *file_info_name;
    string folder_name_root;
    string folder_name_contour;
    string folder_name_yolo;
    string selected_idx;
    Mat merged_img;
public:
    int the_number_of_file_on_grid=100;
    The_LAST_FILTERING(string i_folder_name_root, string i_idx_numb, string i_out_path_contour, string i_out_path_yolo, string i_out_path_parent);
    ~The_LAST_FILTERING();
    bool Creating_Root_Directory();
    bool Creating_Child_Directories();
    void Read_List_of_File_from_Directories();
    void Main_Process();
    int Determine_Process_Index_Numb();
    int Validate_Check(int index);
    int Validate_Check2(int index);
    void Load_Mat_with_Index();
    void Splite_Data_Yolo_and_Contour_v3(string resource_file, string* yolo_data, string* contour_data, string* img_name );
    void Merge_Multi_Mats_to_Single_Mat_v2(Mat resource_mat[],int the_number_of_files);
    string Decision_Part();
    bool Digit_Check(string input_digit);
    void Save_Info_of_Selected_Imgs();
    int Count_the_Number_of_Index(string input_str);
    void Save_Yolo(string img_name, string yolo_data);
    bool Name_Check(string temp_ori_name, string name_ori_img);
    void Save_index_to_int(string input_str, int input_int[]);
    void Save_Index(int index);
    ////filtering version 2
    
};
#endif