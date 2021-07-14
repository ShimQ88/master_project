#ifndef DIRECTORY_CREATER
#define DIRECTORY_CREATER
#include "main.h"
class Machine_Learning_Data_Preparation;
class Parent_ML;

class Directory_Creater{
public:
    string current_full_folder_path_roi;
    string current_full_folder_path_contour;
    string current_full_folder_path_ori;

    // string temp_file_name;
    string folder_name_root;
    string folder_name_parent;
    
    string folder_name_roi;
    string folder_name_contour;
    string folder_name_ori_img;
    string folder_name_yolo;

    string contour_dir;

    int max_numb_of_files_in_folder;
    int current_file_numb;
    int current_folder_numb;
    
    bool is_root_folder_exsist=false;
    bool is_parent_folder_exsist=false;
    bool is_all_folder_exsist=false;

    ofstream contour_file;


    //This is function for contour_yolo_creater.cpp
    Directory_Creater(int i_max_numb_of_files_in_folder, string i_folder_name_root, 
    	string i_folder_name_parent,string i_folder_name_roi, string i_folder_name_contour, string i_folder_name_ori_img);
    Directory_Creater(string i_folder_name_root, string i_folder_name_parent, 
    	string i_folder_name_contour, string i_folder_name_yolo);
    bool Main_Writing_Process_Contour_Yolo_Creater(Mat roi_img, Mat contour_img, Mat ori_img ,string file_name, 
    	string class_numb,float x, float y, float w, float h, string CE);
    bool Info_String_Merge(string class_numb, string temp_file_name,float x, float y, float w, float h, string CE);
    //End This is function for contour_yolo_creater.cpp

    ~Directory_Creater();
    bool Creating_Root_Directory();
    bool Creating_Parent_Directory();
    bool Creating_Child_Directories();
    bool Creating_Grand_Child_Directories();
    bool Creating_Child_Directories_For_The_Second_Filtering();
    string Write_Img(Mat saving_img, string folder_name, string file_name, string file_type);
};

class Write_File{
private:
    Parent_ML *final_ml;
    Machine_Learning_Data_Preparation *prepared_data;
    float mean;
    float variance; 
    float sta_dev; 
    int k_fold_value; 
    Mat *con_mat;
    char **buffer_file;
    string file_full_path;
    string file_the_best_full_path;
    string file_collection_full_path;
    ofstream file;
    ofstream file_the_best;
    ofstream file_collection;
public:
    Write_File();
    Write_File(Parent_ML *i_final_ml, Machine_Learning_Data_Preparation *i_prepared_data,string i_number_of_CE);
    ~Write_File();
    // void Main_Process(float mean, float variance,float sta_dev,int k_fold_value, Mat con_mat[],char **buffer_file);
    void Main_Process();
    void Write_Header(string header);
    string Create_file_path(string file_path, string file_name, string number_of_CE);
    bool The_File_Process();
    bool The_File_Collection_Process(float i_mean_test, float i_sta_dev_test, float i_mean_train, float i_sta_dev_train);
    bool The_Best_Process(float i_mean, float i_variance, float i_sta_dev, int i_k_fold_value,Mat *&i_con_mat, string type);
};


#endif
