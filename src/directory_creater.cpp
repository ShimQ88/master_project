#include "directory_creater.h"

Directory_Creater::Directory_Creater(string i_folder_name_root, string i_folder_name_parent, 
    	string i_folder_name_contour, string i_folder_name_yolo){
	
	folder_name_root=i_folder_name_root;
	folder_name_parent=i_folder_name_parent;
	folder_name_contour=i_folder_name_contour;
	folder_name_yolo=i_folder_name_yolo;

	Creating_Root_Directory();
    Creating_Parent_Directory();
    Creating_Child_Directories_For_The_Second_Filtering();

}
Directory_Creater::Directory_Creater(int i_max_numb_of_files_in_folder, string i_folder_name_root, 
    	string i_folder_name_parent, string i_folder_name_roi, string i_folder_name_contour, string i_folder_name_ori_img){

	// cout<<"correct"<<endl;
    max_numb_of_files_in_folder=i_max_numb_of_files_in_folder;
	folder_name_root=i_folder_name_root;
	folder_name_parent=i_folder_name_parent;

    folder_name_roi=i_folder_name_roi;
    folder_name_contour=i_folder_name_contour;
    folder_name_ori_img=i_folder_name_ori_img;
    current_folder_numb=0;
    current_file_numb=0;
    
    Creating_Root_Directory();
    Creating_Parent_Directory();
    Creating_Child_Directories();
}
        
        

bool Directory_Creater::Main_Writing_Process_Contour_Yolo_Creater(Mat roi_img, Mat contour_img, Mat ori_img ,string file_name,
	string class_numb,float x, float y, float w, float h, string CE){
    int i=0;
    while(1){
        if((is_root_folder_exsist==true)&&(is_all_folder_exsist==true)){break;}//break condition;
        
        is_root_folder_exsist=Creating_Root_Directory();
        is_parent_folder_exsist=Creating_Parent_Directory();
        is_all_folder_exsist=Creating_Child_Directories();
        
        // if(i>105){//happen when loop occur forever
        //     cout<<"error from creating folder"<<endl;
        //     return "false";
        // }
        i++;
    }
    bool file_check;
    if((current_folder_numb==0)&&(current_file_numb==0)){//the program generate the first time
    	while(1){
    		file_check=Creating_Grand_Child_Directories();	
    		if(file_check==false){
    			current_folder_numb++;
    		}else{
    			break;
    		}
    	}
    	contour_dir = folder_name_root+folder_name_parent+folder_name_contour+to_string(current_folder_numb)+"/contour_"+to_string(current_folder_numb)+".txt";
    	contour_file.open(contour_dir);
    }

    Write_Img(roi_img, current_full_folder_path_roi, file_name, "_ROI.jpg");
    Write_Img(contour_img, current_full_folder_path_contour, file_name, "_contour.jpg");
    string temp_file_name=Write_Img(ori_img, current_full_folder_path_ori, file_name, ".jpg");
    
    current_file_numb++;

    bool is_file_reached_max_numb_capacity=Info_String_Merge(class_numb, temp_file_name,x , y, w, h, CE);
    return is_file_reached_max_numb_capacity;
}

bool Directory_Creater::Info_String_Merge(string class_numb, string temp_file_name,float x, float y, float w, float h, string CE){
	// string CE_val=ku_ROI.get_contour_txt();
	bool is_file_reached_max_numb_capacity=false;
	string text_info;
	string final_string=class_numb;
	final_string=final_string+';';
	final_string=final_string+temp_file_name;
	final_string=final_string+';';
	

	string yo_x=to_string(x);
	string yo_y=to_string(y);
	string yo_w=to_string(w);
	string yo_h=to_string(h);
	// cout<<"yo_x: "<<yo_x<<endl;
	// cout<<"float_yo_x: "<<ku_ROI.get_yolo_x()<<endl;
	// final_string=final_string+',';
	final_string=final_string+yo_x;
	// cout<<"final_string: "<<final_string<<endl;
	// cout<<"yo_x: "<<yo_x<<endl;
	final_string=final_string+' ';
	final_string=final_string+yo_y;
	// cout<<"final_string: "<<final_string<<endl;
	final_string=final_string+' ';
	final_string=final_string+yo_w;
	// cout<<"final_string: "<<final_string<<endl;
	final_string=final_string+' ';
	final_string=final_string+yo_h;
	// cout<<"final_string: "<<final_string<<endl;
	final_string=final_string+';';
	final_string=final_string+CE;
    final_string=final_string+"\n";
	// return final_string;
	contour_file << final_string;

	if(current_file_numb==max_numb_of_files_in_folder){
		contour_file.close();
		is_file_reached_max_numb_capacity=true;
	}
	return is_file_reached_max_numb_capacity;
	
}


Directory_Creater::~Directory_Creater(){
}

bool Directory_Creater::Creating_Root_Directory(){
    int size=folder_name_root.length();
    char dirname[size+1];
    strcpy(dirname, folder_name_root.c_str());
    int check = mkdir(dirname,0777);
    
    if(!check){ 
        printf("The folder %s Directory created\n", dirname);
        return false;
    }else{
        printf("Fail to create %s Directory or the folder %s exist\n ", dirname, dirname);
        return true;
    }
}
bool Directory_Creater::Creating_Parent_Directory(){
    string folder_full_path_name=folder_name_root+folder_name_parent;
    int size=folder_full_path_name.length();
    char dirname[size+1];
    strcpy(dirname, folder_full_path_name.c_str());
    int check = mkdir(dirname,0777);
    
    if(!check){ 
        printf("The folder %s Directory created\n", dirname);
        return false;
    }else{
        printf("Fail to create %s Directory or the folder %s exist\n ", dirname, dirname);
        return true;
    }
}

bool Directory_Creater::Creating_Child_Directories_For_The_Second_Filtering(){
    int i=0;
    bool all_file_exsist=true;
    while(1){
        string folder_full_path_name;
        
        if(i==0){
            folder_full_path_name=folder_name_root+folder_name_parent+folder_name_contour;
        }else if(i==1){
            folder_full_path_name=folder_name_root+folder_name_parent+folder_name_yolo;
        }else{
            cout<<"index error"<<endl;
            return false;
        }

        int size=folder_full_path_name.length();

        char dirname[size+1];
        strcpy(dirname, folder_full_path_name.c_str());
        int check = mkdir(dirname,0777);
        
        if(!check){ 
            printf("The folder %s Directory created\n", dirname);
            all_file_exsist=false;
        }else{
            printf("Fail to create %s Directory or the folder %s exist\n ", dirname, dirname);
        }
        if(i==1){break;}//loop breaker
        i++;
    }
    return all_file_exsist;//success making three files
}

bool Directory_Creater::Creating_Child_Directories(){
    int i=0;
    bool all_file_exsist=true;
    while(1){
        string folder_full_path_name;
        
        if(i==0){
            folder_full_path_name=folder_name_root+folder_name_parent+folder_name_roi;
        }else if(i==1){
            folder_full_path_name=folder_name_root+folder_name_parent+folder_name_contour;
        }else if(i==2){
            folder_full_path_name=folder_name_root+folder_name_parent+folder_name_ori_img;
        }else{
            cout<<"index error"<<endl;
            return false;
        }

        int size=folder_full_path_name.length();

        char dirname[size+1];
        strcpy(dirname, folder_full_path_name.c_str());
        int check = mkdir(dirname,0777);
        
        if(!check){ 
            printf("The folder %s Directory created\n", dirname);
            all_file_exsist=false;
        }else{
            printf("Fail to create %s Directory or the folder %s exist\n ", dirname, dirname);
        }
        if(i==2){break;}//loop breaker
        i++;
    }
    return all_file_exsist;//success making three files
}

bool Directory_Creater::Creating_Grand_Child_Directories(){
    int i=0;
    bool is_success_create_file=true;
    while(1){
        string folder_full_path_name;
        
        if(i==0){
            folder_full_path_name=folder_name_root+folder_name_parent+folder_name_roi+to_string(current_folder_numb)+"/";
            current_full_folder_path_roi=folder_full_path_name;
        }else if(i==1){
            folder_full_path_name=folder_name_root+folder_name_parent+folder_name_contour+to_string(current_folder_numb)+"/";
            current_full_folder_path_contour=folder_full_path_name;
        }else if(i==2){
            folder_full_path_name=folder_name_root+folder_name_parent+folder_name_ori_img+to_string(current_folder_numb)+"/";
            current_full_folder_path_ori=folder_full_path_name;
        }else{
            cout<<"index error"<<endl;
            // return false;
            is_success_create_file=false;
        }
        // cout<<"current_full_folder_path_roi:"<<current_full_folder_path_roi<<endl;
        // getchar();
        
        int size=folder_full_path_name.length();

        char dirname[size+1];
        strcpy(dirname, folder_full_path_name.c_str());
        int check = mkdir(dirname,0777);
        
        if(!check){ 
            printf("The folder %s Directory created\n", dirname);
            
        }else{
            printf("Fail to create %s Directory or the folder %s exist\n ", dirname, dirname);
            // current_folder_numb++;
            // continue;
            is_success_create_file=false;
        }
        if(i==2){break;}//loop breaker
        i++;
    }
    return is_success_create_file;


}
string Directory_Creater::Write_Img(Mat saving_img, string folder_name, string file_name, string file_type){
    int delimiter=0;
    while(delimiter!=-1){
        delimiter = file_name.find('/');
        file_name = file_name.substr(delimiter+1);
    }
    file_name = file_name.substr(0,file_name.find('.'));
    // cout<<"file_name: "<<file_name<<endl;

    string output_name=file_name+file_type;
    string path_ROI=folder_name+file_name+file_type;

    imwrite( path_ROI, saving_img);
    return output_name;
}



Write_File::Write_File(Parent_ML *i_final_ml, Machine_Learning_Data_Preparation *i_prepared_data, string i_number_of_CE){
    final_ml=i_final_ml;
    prepared_data=i_prepared_data;
    // mean=final_ml->mean;
    // variance=final_ml->variance; 
    // sta_dev=final_ml->sta_dev;
    // k_fold_value=prepared_data->k_fold_value;
    // con_mat=final_ml->confusion_matrix;
    // buffer_file=final_ml->result_buffer;
    // cout<<"i_number_of_CE: "<<i_number_of_CE<<endl;
    string file_path="resource/";
    // file_full_path=Create_file_path(file_path,"min_sample_count_",i_number_of_CE);
    file_the_best_full_path=Create_file_path(file_path,"Calculate_standard_deviation_",i_number_of_CE);
    file_collection_full_path=Create_file_path(file_path,"accuracy_collection_",i_number_of_CE);

    // file.open(file_full_path, std::ios_base::app);
    file_collection.open(file_collection_full_path, std::ios_base::app);
    file_the_best.open(file_the_best_full_path, std::ios_base::app);

}

Write_File::~Write_File(){
    file.close();
    file_the_best.close();
    file_collection.close();
}
void Write_File::Write_Header(string header){
    file_collection<<header;
    file_collection<<endl;
    // file<<final_ml->Head_Parameter();
    file<<endl;
}
void Write_File::Main_Process(){
    // The_Best_Process();
    The_File_Collection_Process(final_ml->mean_test, final_ml->sta_dev_test, final_ml->mean_train, final_ml->sta_dev_train);
    // The_File_Process();

    // cout<<"final_ml->mean_test: "<<final_ml->mean_test<<endl;
    // cout<<"final_ml->variance_test: "<<final_ml->variance_test<<endl;
    // cout<<"final_ml->sta_dev_test: "<<final_ml->sta_dev_test<<endl;
    // cout<<"prepared_data->k_fold_value: "<<prepared_data->k_fold_value<<endl;
    // cout<<"final_ml->confusion_matrix_test: "<<final_ml->confusion_matrix_test<<endl;


    The_Best_Process(final_ml->mean_test, final_ml->variance_test, final_ml->sta_dev_test, 
        prepared_data->k_fold_value, final_ml->confusion_matrix_test, "test");
    The_Best_Process(final_ml->mean_train, final_ml->variance_train, final_ml->sta_dev_train, 
        prepared_data->k_fold_value, final_ml->confusion_matrix_train, "train");

}

string Write_File::Create_file_path(string file_path, string file_name, string number_of_CE){
    string file_name_extension=".txt";
    string full_file_name=file_name+number_of_CE+file_name_extension;
    string temp_file_full_path=file_path+full_file_name;
    cout<<"temp_file_full_path: "<<temp_file_full_path<<endl; 
    return temp_file_full_path;
}

bool Write_File::The_File_Process(){
    for(int i=0;i<k_fold_value;i++){
        file<<buffer_file[i];
    }
    return true;        
}

bool Write_File::The_File_Collection_Process(float i_mean_test, float i_sta_dev_test, float i_mean_train, float i_sta_dev_train){
    // char mse_buffer_test[70];
    // char mse_buffer_train[70];
    // sprintf(mse_buffer_test, "%1.f ± %1.f%% \n", i_mean_test*100, i_sta_dev_test*100);
    // sprintf(mse_buffer_train, "%1.f ± %1.f%% \n", i_mean_train*100, i_sta_dev_train*100);
    // file_collection<<"#Mean, Variance, Sta_dev, Mean Square Error";
    file_collection<<final_ml->result_buffer;
    // file_collection<<endl;
    // file_collection<<to_string(mean);
    // file_collection<<", ";
    // file_collection<<to_string(variance);
    // file_collection<<", ";
    // file_collection<<to_string(sta_dev);
    // file_collection<<", ";
    // file_collection<<mse_buffer;
    // file_collection<<endl;
    // file_collection<<mse_buffer_test;
    // file_collection<<", ";
    // file_collection<<to_string(i_mean_test);
    // file_collection<<endl; 
    
    return true;
}

bool Write_File::The_Best_Process(float i_mean, float i_variance, float i_sta_dev, int i_k_fold_value, Mat *&i_con_mat, string type){
    char mean_buffer[20],variance_buffer[40],sta_dev_buffer[40],mse_buffer[70];
    sprintf(mean_buffer, "#mean: %f \n", i_mean);
    sprintf(variance_buffer, "#variance: %f \n", i_variance);
    sprintf(sta_dev_buffer, "#sta_dev: %f \n", i_sta_dev);  //header
    sprintf(mse_buffer, "#Mean Square Error: %1.f ± %1.f%% \n", i_mean*100, i_sta_dev*100);
    cout<<"start Best"<<endl;
    cout<<"mean:"<<mean_buffer<<endl;
    cout<<"variance_buffer:"<<variance_buffer<<endl;
    cout<<"sta_dev_buffer:"<<sta_dev_buffer<<endl;
    cout<<"mse_buffer:"<<mse_buffer<<endl;
    cout<<"end Best"<<endl;
    file_the_best << "----------";
    file_the_best << type;
    file_the_best << "----------\n";
    if (file_the_best){
        // file_the_best<<"\n\n";    
        file_the_best<<mean_buffer;
        file_the_best<<variance_buffer;
        file_the_best<<sta_dev_buffer;
        file_the_best<<mse_buffer;
        file_the_best<<"\n\n";
        file_the_best<<"#Confusion Matrix\n";
        for(int i=0; i<i_k_fold_value; i++){
            char buffer[50];
            sprintf(buffer, "#k=%d\n", i);  //header
            file_the_best<<buffer;
            file_the_best<<"#";
            file_the_best<<i_con_mat[i].at<int>(0,0);
            file_the_best<<", ";
            file_the_best<<i_con_mat[i].at<int>(0,1);
            file_the_best<<"\n";
            file_the_best<<"#";
            file_the_best<<i_con_mat[i].at<int>(1,0);
            file_the_best<<", ";
            file_the_best<<i_con_mat[i].at<int>(1,1);
            file_the_best<<"\n\n";
        }
            
    }
    file_the_best << "--------";
    file_the_best << type;
    file_the_best << " end";
    file_the_best << "--------\n";
    return 0;
}