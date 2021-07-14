#include "the_second_cycling.h"

bool Info_Save(string path, string info){
	//index save area
	ofstream index_save;
	index_save.open (path);
	index_save << info;
	index_save.close();
	return true;
}
// string Extract_File_name_From_Full_Path(string name){
// 	int delimiter=0;
// 	string output_file_name;
// 	while(delimiter!=-1){
// 		delimiter = name.find('/');
// 		name=name.substr(delimiter+1);
// 	}
// 	output_file_name=name;
// 	return output_file_name;
// }
string Name_Change_txt_to_jpg(string input_name){
    input_name=Extract_File_name_From_Full_Path(input_name);
    // cout<<"input_name: "<<input_name<<endl;
    
    int delimiter=0;
    while(1){//The process to remove extensive from the name full name 
        delimiter = input_name.find('.');
        if(delimiter==-1){break;}
        input_name = input_name.substr(0, delimiter);
        
        // cout<<"temp_name: "<<temp_name<<endl;
        // input_name = input_name.substr(delimiter+1);

        // char digit_str[temp_digit.length()];
        // for(int i=0;i<temp_digit.length();i++){
        //     char digit_str[1];
        //     digit_str[0]=temp_digit[i];
        //     if(isdigit(digit_str[0])==false){
        //         return false;
        //     }
        // }
    }
    string output_name=input_name+".jpg";
    return output_name;   
}


void Extract_File_Name_From_Contours(string path_contour_file, vector<string>& imgname){
	ifstream ce_load;
    ce_load.open(path_contour_file);
    
    string read_line;
    if(ce_load.is_open()){
		while(getline(ce_load, read_line)){
			int delimiter=0;
			string name;
			delimiter = read_line.find(',');
			read_line=read_line.substr(delimiter+1);
			delimiter = read_line.find(',');//image name located in second
			name=read_line.substr(0,delimiter);
			imgname.push_back(name);
			// cout<<"name: "<<name<<endl;
		}
	}
}

bool Scan_File_From_Contour_File(string input_file, vector<string> imgname){
	bool is_find=false;
    for (auto i = imgname.begin(); i != imgname.end(); ++i){
    	string name;
    	// cout<<*i<<endl;
        int delimiter = i->find('.');
        name=i->substr(0,delimiter);
        
        string name1=name+".jpg";
        string name2=name+".JPG";
        // cout<<"name1,name2:"<<name1<<", "<<name2<<endl;
        if(input_file.compare(name1)==0){
            is_find=true;
        }

        if(input_file.compare(name2)==0){
            is_find=true;
        }
        if(is_find==true){
        	break;
        }
    }
	return is_find;
}
YOLO_TXT_INFO Extract_Yolo_txt_Info(string line){
    int delimiter=0;
    int idx=0;
    YOLO_TXT_INFO Yolo_info;
    while(1){//The process to remove extensive from the name full name 
        delimiter = line.find(',');
        if(delimiter==-1){break;}
        
        string extracted_info = line.substr(0,delimiter);
        
        line = line.substr(delimiter+1);

        if(idx==0){
            Yolo_info.class_numb=extracted_info;
        }else if(idx==1){
            Yolo_info.accuracy=stof(extracted_info);
        }else if(idx==2){
            Yolo_info.p1.x=stof(extracted_info);
        }else if(idx==3){
            Yolo_info.p1.y=stof(extracted_info);
        }else if(idx==4){
            Yolo_info.p2.x=stof(extracted_info);
        }else{

        }
        idx++;
    }
    Yolo_info.p2.y=stof(line);
    return Yolo_info;
}
YOLO_TXT_INFO Extract_Yolo_txt_Info2(string line){
    int delimiter=0;
    int idx=0;
    YOLO_TXT_INFO Yolo_info;
    while(1){//The process to remove extensive from the name full name 
        delimiter = line.find(' ');
        if(delimiter==-1){break;}
        
        string extracted_info = line.substr(0,delimiter);
        
        line = line.substr(delimiter+1);
        cout<<"extracted_info: "<<extracted_info<<endl;
        if(idx==0){
            Yolo_info.class_numb=extracted_info;
        }else if(idx==1){
            Yolo_info.p1.x=stof(extracted_info);
        }else if(idx==2){
            Yolo_info.p1.y=stof(extracted_info);
        }else if(idx==3){
            Yolo_info.p2.x=stof(extracted_info);
        }else{

        }
        idx++;
    }
    Yolo_info.p2.y=stof(line);
    return Yolo_info;
}

YOLO_TXT_INFO Read_txt_File_Store_Yolo_Info(string file_path){//This is not working if detected over 200
    ifstream file;
    file.open(file_path);
    int idx=0;
    YOLO_TXT_INFO yolo_info[200];
    YOLO_TXT_INFO output_yolo_info;
    if( file.is_open() ){
        string line;
        while ( getline(file,line) ){
            yolo_info[idx]=Extract_Yolo_txt_Info(line);
            // cout<<"line: "<<line<<endl;
            // cout<<"yolo_info.class_numb: "<<yolo_info.class_numb<<endl;
            // cout<<"yolo_info.accuracy: "<<yolo_info.accuracy<<endl;
            // cout<<"yolo_info.p1.x: "<<yolo_info.p1.x<<endl;
            // cout<<"yolo_info.p1.y: "<<yolo_info.p1.y<<endl;
            // cout<<"yolo_info.p2.x: "<<yolo_info.p2.x<<endl;
            // cout<<"yolo_info.p2.y: "<<yolo_info.p2.y<<endl;
            // getchar();
            idx++;
        }
        if(idx==1){
            // cout<<"The single line"<<endl;
            output_yolo_info=yolo_info[0];
        }else{
            // cout<<"The multiple line"<<endl;
            // getchar();
            int best_accuracy_idx=0;
            float the_highest_accuracy=-1;
            for(int i=0;i<idx;i++){
                if( the_highest_accuracy<yolo_info[i].accuracy ){
                    the_highest_accuracy=yolo_info[i].accuracy;
                    best_accuracy_idx=i;
                }

            }
            output_yolo_info=yolo_info[best_accuracy_idx];
        }
    }else{

    }
    // cout<<"idx: "<<idx<<endl;
    // cout<<"class: "<<output_yolo_info.class_numb<<endl;
    // cout<<"accuracy: "<<output_yolo_info.accuracy<<endl;
    // cout<<".p1.x: "<<output_yolo_info.p1.x<<endl;
    return output_yolo_info;
}

YOLO_TXT_INFO Read_txt_File_Store_Yolo_Info2(string file_path){//This is not working if detected over 200
    ifstream file;
    file.open(file_path);
    int idx=0;
    YOLO_TXT_INFO yolo_info[200];
    YOLO_TXT_INFO output_yolo_info;
    if( file.is_open() ){
        string line;
        while ( getline(file,line) ){
            yolo_info[idx]=Extract_Yolo_txt_Info2(line);
            cout<<"line: "<<line<<endl;
            cout<<yolo_info[idx].class_numb<<endl;
            // cout<<(float)yolo_info[idx].p1.x<<endl;
            // cout<<(float)yolo_info[idx].p1.y<<endl;
            // cout<<(float)yolo_info[idx].p2.x<<endl;
            // cout<<(float)yolo_info[idx].p2.y<<endl;
            // cout<<"yolo_info.class_numb: "<<yolo_info.class_numb<<endl;
            // cout<<"yolo_info.accuracy: "<<yolo_info.accuracy<<endl;
            // cout<<"yolo_info.p1.x: "<<yolo_info.p1.x<<endl;
            // cout<<"yolo_info.p1.y: "<<yolo_info.p1.y<<endl;
            // cout<<"yolo_info.p2.x: "<<yolo_info.p2.x<<endl;
            // cout<<"yolo_info.p2.y: "<<yolo_info.p2.y<<endl;
            // getchar();
            idx++;
        }
        if(idx==1){
            // cout<<"The single line"<<endl;
            output_yolo_info=yolo_info[0];
        }
    }else{

    }
    // cout<<"idx: "<<idx<<endl;
    // cout<<"class: "<<output_yolo_info.class_numb<<endl;
    // cout<<".p1.x: "<<output_yolo_info.p1.x<<endl;
    // cout<<".p1.y: "<<output_yolo_info.p1.y<<endl;
    // cout<<".p2.x: "<<output_yolo_info.p2.x<<endl;
    // cout<<".p2.y: "<<output_yolo_info.p2.y<<endl;
    return output_yolo_info;
}
bool Read_Original_Image(string& full_path, Mat& output_ori_img){
    output_ori_img=imread(full_path,1);
    if( output_ori_img.empty()==false ){
        return true;
    }else{

        int delimiter=0;
        int idx=0;

        delimiter = full_path.find('.');
        
        string extracted_name_without_ext = full_path.substr(0,delimiter);

        string extracted_name_with_ext;
        bool success=false;
        for(int i=0;i<4;i++){
            string ext;
            if(i==0){
                ext=".JPG";
            }else if(i==1){
                ext=".jpeg";
            }else if(i==2){
                ext=".JPEG";
            }else if(i==3){
                ext=".bmp";
            }else{//in case
                ext=".jpg";
            }
            extracted_name_with_ext=extracted_name_without_ext+ext;
            output_ori_img=imread(extracted_name_with_ext,1);
            if( output_ori_img.empty()==false ){
                success=true;
                break;
            }
        }
        if(success==true){
            full_path=extracted_name_with_ext;
            return true;
        }
    }
    return false;
}

void Return_Prev_and_Next_Full_File_Path(string input_name, string& prev_name, string& next_name){    

    //the process extract extension
    int delimiter = input_name.find('.');
    string ext=input_name.substr(delimiter);
    input_name=input_name.substr(0,delimiter);
    // cout<<"full name: "<<input_name<<endl;
    // cout<<"ext: "<<ext<<endl;
    ////////////////////////////

    // string extracted_numb=input_name.substr(input_name.size()-6,2);
    string extracted_numb=input_name.substr(input_name.size()-3,3);
    string extracted_file_info=input_name.substr(0,input_name.size()-3);
    // input_name.substr(input_name.size()-6,2);
    input_name.substr(input_name.size(),3);

    // cout<<"extracted_file_info: "<<extracted_file_info<<endl;
    // cout<<"extracted_numb: "<<extracted_numb<<endl;
    int numb=stoi(extracted_numb);
    int next_numb;
    int prev_numb;
    string prev_file;
    string next_file;
    if((numb==0)||(numb==999)){//the case of 4digits required
        extracted_numb=input_name.substr(input_name.size()-4,4);
        extracted_file_info=input_name.substr(0,input_name.size()-4);
        numb=stoi(extracted_numb);
        next_numb=numb+1;
        prev_numb=numb-1;
        prev_file=extracted_file_info+to_string(prev_numb)+ext;
        next_file=extracted_file_info+to_string(next_numb)+ext;
    }else{
        next_numb=numb+1;
        prev_numb=numb-1;

        //when the value under 10 name affect
        if(prev_numb<10){
            prev_file=extracted_file_info+"00"+to_string(prev_numb)+ext;
        }else if((prev_numb>=10)&&(prev_numb<100)){
            prev_file=extracted_file_info+"0"+to_string(prev_numb)+ext;
        }else{
            prev_file=extracted_file_info+to_string(prev_numb)+ext;
        }

        if(next_numb<10){
            next_file=extracted_file_info+"00"+to_string(next_numb)+ext;
        }else if((next_numb>=10)&&(next_numb<100)){
            next_file=extracted_file_info+"0"+to_string(next_numb)+ext;
        }else{
            next_file=extracted_file_info+to_string(next_numb)+ext;
        }
    }
    
 
    
    
    // cout<<"prev_numb: "<<prev_numb<<endl;
    // cout<<"next_numb: "<<next_numb<<endl;
    
    

    // cout<<"prev_file: "<<prev_file<<endl;
    // cout<<"next_file: "<<next_file<<endl;
    // getchar();
    // string full_path_next_file=Dataset_Path+next_file;
    // string full_path_prev_file=Dataset_Path+prev_file;

    prev_name=prev_file;
    next_name=next_file;
    
}


void Main_Contour_Yolo_Creater_With_Window_Info(const string class_number, const string path_dataset, const string idx_value, const string path_contour_file, const string path_yolov5, const string save_location){
	
	// cout<<path_extra_need+"path_yolov5.txt"<<endl;
	// cout<<path_extra_need+"file_name.txt"<<endl;
	if(idx_value=="0"){
		string path_extra_need="extra_script_for_the_second_cycle/";
		Info_Save(path_extra_need+"path_yolov5.txt",path_yolov5);
		Info_Save(path_extra_need+"file_name.txt",path_dataset);
		// system("bash extra_script_for_the_second_cycle/script_for_the_second_cycle.sh");
	}
	
	// ofstream ce_save;
 //    ce_save.open("contour_CEs2.txt");
    // glob_t glob_result=Read_Dataset_Dir(path_all_class_dataset[class_value]);
    vector<string> imgname;
    // cout<<"contour path:"<< path_contour_file<<endl;
    string temp_name=path_dataset+"";
    // glob_t glob_result=Read_Dataset_Dir(path_dataset);
    // for(i=stoi(idx_value); i<glob_result.gl_pathc; i++){

    // }
    string filename_parent=Extract_File_name_From_Full_Path(path_dataset);
    if(filename_parent==""){
    	filename_parent="default";
    }
    // cout<<"filename_parent "<<filename_parent<<endl;
    // getchar();
	filename_parent=filename_parent+"/";
	// cout<<"filename_parent: "<<filename_parent<<endl;
    // string folder_name_root=save_location+"/"+"the_second_cycle/";
    string folder_name_root="the_second_cycle/";
    string folder_name_roi="roi/";
	string folder_name_contour="contour/";
    string folder_name_ori_img="image/";
	Directory_Creater write_file(the_number_of_file_in_folder,
		folder_name_root,
		 filename_parent,
		 folder_name_roi,
		 folder_name_contour,
		  folder_name_ori_img);
	bool are_files_reached_max_capacity=false;

    Extract_File_Name_From_Contours(path_contour_file,imgname);
    glob_t glob_result=Read_Dataset_Dir("txt_file_from_contour/");
    YOLO_TXT_INFO yolo_info;
    unsigned int j;//in order to save index later
    for(j=stoi(idx_value); j<glob_result.gl_pathc; j++){
    	
    	string temp_filename_txt=Extract_File_name_From_Full_Path(glob_result.gl_pathv[j]);
    	string temp_filename_jpg=Name_Change_txt_to_jpg(temp_filename_txt);

    	// cout<<"temp_filename2: "<<temp_filename2<<endl;

    	bool is_find=Scan_File_From_Contour_File(temp_filename_jpg,imgname);
    	

    	if(is_find==true){//the case file found from the contour file
    		// cout<<"find"<<endl;
    		//skip in this time
    		continue;
    	}else{
    		// cout<<"no"<<endl;
    		// cout<<"glob_result.gl_pathv[j]: "<<glob_result.gl_pathv[j]<<endl;
    		yolo_info=Read_txt_File_Store_Yolo_Info(glob_result.gl_pathv[j]);
    		if(yolo_info.accuracy<=0.6){
	            cout<<"low accuracy skip"<<endl;
	            // getchar();
	            // continue;
	        }//the second exception
	        // cout<<"p3"<<endl;
	        // cout<<"yolo_info.class_numb: "<<yolo_info.class_numb<<endl;
	        if(yolo_info.class_numb!=class_number){//wrong prediction
	            cout<<"Wrong Prediction"<<endl;
	            // getchar();
	            // continue;
	        }else{//the thrid exception
	        	continue;
	        }
	        Mat ori_img;
	        string full_path=path_dataset+"/"+temp_filename_jpg;
	        bool is_successfully_loaded_ori_img=Read_Original_Image(full_path, ori_img);
	        if(is_successfully_loaded_ori_img==false){
	            cout<<"fail to load original image"<<endl;
	            continue;
	        }else{
	        	// cout<<"Great"<<endl;

	        }

	        // resize(ori_img, ori_img, cv::Size(640,640));//resized
	        // YOLO_IMAGES output_yolo_images;
	       
	        string prev_name, next_name;

	        Return_Prev_and_Next_Full_File_Path(full_path, prev_name, next_name);
	        Fourier_Descriptors FD;
	        bool is_success=FD.Main_The_Second_Cycle_Process(full_path, prev_name, next_name,yolo_info);
	     //    bool is_success=FD.Main_The_Second_Cycle_Process2(p_target_file_name,p_prev_file_name, p_next_file_name, YOLO_TXT_INFO yolo_info);
		    
		    
		    if(is_success==true){
				are_files_reached_max_capacity=
		  		write_file.Main_Writing_Process_Contour_Yolo_Creater(
		  			FD.target_ROI,
		  			FD.ROI_contour,
		  			FD.target_color_img,
		  			FD.target_file_name,
		  			class_number,	
		  			FD.YL.yolo_x,
		  			FD.YL.yolo_y,
		  			FD.YL.yolo_width,
		  			FD.YL.yolo_height,
		  			FD.contour_txt
		  			);	

	  			if(are_files_reached_max_capacity==true){break;}
			}


		    // Mat target_ROI=ori_img(window_frame).clone();
		    // cout<<"target_ROI.rows:"<<target_ROI.rows<<endl;
		    // cout<<"target_ROI.cols:"<<target_ROI.cols<<endl;
		    // // Mat ROI=ori_img(window_frame).clone();
		    // imshow("target_ROI",target_ROI);
		    // waitKey(0);

    	}
    }
    j++;//next one
	bool is_success_to_save_index=Index_Saver(folder_name_root+filename_parent+"/index.txt",j);	

    // Scan_File_From_Contour_File(path_contour_file,imgname);

    

    	// std::cout << *i << endl;

    // bool ok = Read_Numb_Class_Data( path_shuffled_data, numb_of_data_cols, &data, &responses, &imgname);//third parameter: FEATURES
    // cout<<"ok: "<<ok<<endl;
	// ofstream ce_save;
	// for(int j=0; j<glob_result.gl_pathc; j++){

	// }
}

void Main_Contour_Yolo_Creater_With_Window_Info_Video(const string class_number, const string path_dataset, const string path_txt, const string idx_value, const string path_contour_file, const string path_yolov5, const string save_location){
    
    // cout<<path_extra_need+"path_yolov5.txt"<<endl;
    // cout<<path_extra_need+"file_name.txt"<<endl;
    if(idx_value=="0"){
        string path_extra_need="extra_script_for_the_second_cycle/";
        Info_Save(path_extra_need+"path_yolov5.txt",path_yolov5);
        Info_Save(path_extra_need+"file_name.txt",path_dataset);
        // system("bash extra_script_for_the_second_cycle/script_for_the_second_cycle.sh");
    }
    
    // ofstream ce_save;
 //    ce_save.open("contour_CEs2.txt");
    // glob_t glob_result=Read_Dataset_Dir(path_all_class_dataset[class_value]);
    vector<string> imgname;
    // cout<<"contour path:"<< path_contour_file<<endl;
    string temp_name=path_dataset+"";
    // glob_t glob_result=Read_Dataset_Dir(path_dataset);
    // for(i=stoi(idx_value); i<glob_result.gl_pathc; i++){

    // }
    string filename_parent=Extract_File_name_From_Full_Path(path_dataset);
    if(filename_parent==""){
        filename_parent="default";
    }
    // cout<<"filename_parent "<<filename_parent<<endl;
    // getchar();
    filename_parent=filename_parent+"/";
    // cout<<"filename_parent: "<<filename_parent<<endl;
    // string folder_name_root=save_location+"/"+"the_second_cycle/";
    string folder_name_root="the_second_cycle/";
    string folder_name_roi="roi/";
    string folder_name_contour="contour/";
    string folder_name_ori_img="image/";
    Directory_Creater write_file(the_number_of_file_in_folder,
        folder_name_root,
         filename_parent,
         folder_name_roi,
         folder_name_contour,
          folder_name_ori_img);
    bool are_files_reached_max_capacity=false;

    Extract_File_Name_From_Contours(path_contour_file,imgname);
    glob_t glob_result;
    if(path_txt==""){
        glob_result=Read_Dataset_Dir("txt_file_from_contour/");
    }else{
        glob_result=Read_Dataset_Dir(path_txt);
    }
    
    YOLO_TXT_INFO yolo_info;
    unsigned int j;//in order to save index later
    for(j=stoi(idx_value); j<glob_result.gl_pathc; j++){
        if(j==0){
            continue;
        }
        string temp_filename_txt_prev=Extract_File_name_From_Full_Path(glob_result.gl_pathv[0]);
        string temp_filename_jpg_prev=Name_Change_txt_to_jpg(temp_filename_txt_prev);


        string temp_filename_txt=Extract_File_name_From_Full_Path(glob_result.gl_pathv[j]);
        string temp_filename_jpg=Name_Change_txt_to_jpg(temp_filename_txt);

        // cout<<"temp_filename2: "<<temp_filename2<<endl;

        bool is_find=Scan_File_From_Contour_File(temp_filename_jpg,imgname);
        

        if(is_find==true){//the case file found from the contour file
            // cout<<"find"<<endl;
            //skip in this time
            continue;
        }else{
            // cout<<"no"<<endl;
            // cout<<"glob_result.gl_pathv[j]: "<<glob_result.gl_pathv[j]<<endl;
            yolo_info=Read_txt_File_Store_Yolo_Info(glob_result.gl_pathv[j]);
            if(yolo_info.accuracy<=0.6){
                cout<<"low accuracy skip"<<endl;
                // getchar();
                // continue;
            }//the second exception
            // cout<<"p3"<<endl;
            // cout<<"yolo_info.class_numb: "<<yolo_info.class_numb<<endl;
            // cout<<"yolo_info.accuracy: "<<yolo_info.accuracy<<endl;
            if(yolo_info.class_numb!=class_number){//wrong prediction
                cout<<"Wrong Prediction"<<endl;
                // getchar();
                // continue;
            }else{//the thrid exception
                // continue;
            }
            Mat ori_img;
            string full_path=path_dataset+"/"+temp_filename_jpg;
            string full_path_prev=path_dataset+"/"+temp_filename_jpg_prev;
            cout<<"full_path_prev: "<<full_path_prev<<endl;
            cout<<"full_path: "<<full_path<<endl;
            bool is_successfully_loaded_ori_img=Read_Original_Image(full_path, ori_img);
            if(is_successfully_loaded_ori_img==false){
                cout<<"fail to load original image"<<endl;
                continue;
            }else{
                // cout<<"Great"<<endl;

            }
            // getchar();
            Fourier_Descriptors FD;
            bool is_success=FD.Main_The_Video_Cycle_Process(full_path, full_path_prev, "", yolo_info);
            
            
            if(is_success==true){
                are_files_reached_max_capacity=
                write_file.Main_Writing_Process_Contour_Yolo_Creater(
                    FD.target_ROI,
                    FD.ROI_contour,
                    FD.target_color_img,
                    FD.target_file_name,
                    class_number,   
                    FD.YL.yolo_x,
                    FD.YL.yolo_y,
                    FD.YL.yolo_width,
                    FD.YL.yolo_height,
                    FD.contour_txt
                    );  

                if(are_files_reached_max_capacity==true){break;}
            }
            // getchar();

            // Mat target_ROI=ori_img(window_frame).clone();
            // cout<<"target_ROI.rows:"<<target_ROI.rows<<endl;
            // cout<<"target_ROI.cols:"<<target_ROI.cols<<endl;
            // // Mat ROI=ori_img(window_frame).clone();
            // imshow("target_ROI",target_ROI);
            // waitKey(0);

        }
    }
    j++;//next one
    bool is_success_to_save_index=Index_Saver(folder_name_root+filename_parent+"/index.txt",j); 

    // Scan_File_From_Contour_File(path_contour_file,imgname);

    

        // std::cout << *i << endl;

    // bool ok = Read_Numb_Class_Data( path_shuffled_data, numb_of_data_cols, &data, &responses, &imgname);//third parameter: FEATURES
    // cout<<"ok: "<<ok<<endl;
    // ofstream ce_save;
    // for(int j=0; j<glob_result.gl_pathc; j++){

    // }
}