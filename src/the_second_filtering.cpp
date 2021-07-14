#include "the_second_filtering.h"
#include "directory_creater.h"

The_Second_Filtering::The_Second_Filtering(string i_folder_name_root, string i_idx_numb, string i_out_path_contour, string i_out_path_yolo, string i_out_path_parent ){
	path_images=i_folder_name_root;
	idx=stoi(i_idx_numb);
    out_path_contour=i_out_path_contour;
    out_path_yolo=i_out_path_yolo;
    out_path_parent=i_out_path_parent;
    // cout<<"out_path_contour: "<<out_path_contour<<endl;
    // cout<<"out_path_parent: "<<out_path_parent<<endl;
    // cout<<"out_path_yolo: "<<out_path_yolo<<endl;
    // getchar();
}

The_Second_Filtering::~The_Second_Filtering(){
    cout<<"Deconstruction called"<<endl;
    delete[] final_path_contour;
    delete[] final_path_yolo;
    // delete parent_path_contour;
    // delete parent_path_ori_img;
    delete[] image_contour;
    delete[] image_original;
    delete[] file_read;
    // getchar();
}

void The_Second_Filtering::Main_Process(){
    bool path_check=Determine_Final_Path_Contour(path_images, idx_numb);
    if(path_check==false){
        cout<<"Path problem"<<endl;
        return;
    }
    // cout<<"p1"<<endl;
    // bool is_the_number_of_files_correct=Load_Img_and_Store_to_Mat();
    //in case error when txt is located in the last of folder. 

    bool is_the_number_of_files_correct=Load_Img_and_Store_to_Mat_V2();
    // cout<<"haha"<<endl;
    //in case error when txt is located in the first of folder. 
    if(is_the_number_of_files_correct==false){
        cout<<"The second problem"<<endl;
        return;
    }
    // cout<<"p2"<<endl;
    Merge_Multi_Mats_to_Single_Mat_v2(image_contour,the_number_of_files);
    // cout<<"p3"<<endl;
    string decision=Decision_Part();
    if(decision.compare("ND")==0){//I made this one owing to avoiding mistake input
        cout<<"input Error try again: Press enter"<<endl;
        
    }else if(decision.compare("skip")==0){
        cout<<"skip and looking next folder"<<endl;
        ofstream index_save;
        string idx_path=out_path_parent+"/index.txt";
        index_save.open(idx_path);
        index_save<<to_string(++idx);
        index_save.close();

    }
    if(decision.compare("y")==0){//I made this one owing to avoiding mistake input
        Read_Info_Contour_Yolo();
        Save_Info_of_Selected_Imgs();
    }
    
    
    // cout<<"parent_path_contour:"<<parent_path_contour<<endl;
    // cout<<"parent_path_ori_img:"<<parent_path_ori_img<<endl;
    // cout<<"final_path_contour:"<<final_path_contour<<endl;
    // cout<<"final_path_yolo:"<<final_path_yolo<<endl;
    cout<<"done"<<endl;
}

string The_Second_Filtering::Decision_Part(){
    imshow("merged_img", merged_img);
    waitKey(0);
    cout << "Choose what you want: ";
    cin >> selected_idx; // get user input from the keyboard
    if(selected_idx.compare("s")==0){
        return "skip";
    }
    cout << "You choose: " << selected_idx <<endl;
    bool is_input_error=Digit_Check(selected_idx);
    if(is_input_error==false){
        cout << "The input error: " << selected_idx <<endl;
        return "ND";
    }

    cout<<"Are you sure for saving those datas?"<<endl;
    cout<<"y to continue, any button to finish this program: ";
    string decision;
    cin >> decision;
    return decision;
}

bool The_Second_Filtering::Determine_Final_Path_Contour(string i_path_images, string i_idx_numb){
    char temp_path[i_path_images.size()+2];//if could not read change the number of 10
	for(int i=0;i<i_path_images.size()+2;i++){
		temp_path[i]=i_path_images[i];
	}

    ////determine path
    parent_path_contour=new char[i_path_images.size()+10];
    parent_path_ori_img=new char[i_path_images.size()+10];
    strcpy(parent_path_contour, temp_path);    
    strcat(parent_path_contour,"/contour/");

    strcpy(parent_path_ori_img, temp_path);
    strcat(parent_path_ori_img,"/image/");

    //////

    // idx=Determine_Folder_Numb(i_idx_numb);
    // idx=Determine_Folder_Numb(i_idx_numb);
    string index_str=to_string(idx);
    
    int size_contour=strlen(parent_path_contour);
    int size_ori_img=strlen(parent_path_ori_img);
    
    final_path_contour=new char[size_contour+index_str.length()+4];
    final_path_yolo=new char[size_ori_img+index_str.length()+4];
    
    //process string to char
    char temp_char[index_str.length()]; 
    
    for (int j=0; j<sizeof(temp_char);j++) { 
        temp_char[j] = index_str[j]; 
    } 

    strcat(parent_path_contour, temp_char);
    strcat(parent_path_ori_img, temp_char);

    strcpy(final_path_contour, parent_path_contour);
    strcpy(final_path_yolo, parent_path_ori_img);

    bool is_path_exist=Path_Check();

    strcat(final_path_contour,"/*");
    strcat(final_path_yolo,"/*"); 

    //print for test
 //    cout<<"final_path_contour: ";
 //    for(int i=0;i<strlen(final_path_contour);i++){
	// 	cout<<final_path_contour[i];
	// }
	// cout<<endl;
 //    cout<<"final_path_yolo: ";
 //    for(int i=0;i<strlen(final_path_yolo);i++){
	// 	cout<<final_path_yolo[i];
	// }
	// cout<<endl;

    return is_path_exist;
    
    
}
bool The_Second_Filtering::Path_Check(){
    bool is_path_exist=true;
    struct stat info;
    if( stat( final_path_contour, &info ) != 0 ){
        printf( "cannot access %s\n", final_path_contour );
        is_path_exist=false;
    }else if( info.st_mode & S_IFDIR ){  // S_ISDIR() doesn't exist on my windows 
        printf( "%s is a directory\n", final_path_contour );
    }else{
        printf( "%s is no directory\n", final_path_contour );
        // is_path_exist=false;
    }

    if( stat( final_path_yolo, &info ) != 0 ){
        printf( "cannot access %s\n", final_path_yolo );
        is_path_exist=false;
    }else if( info.st_mode & S_IFDIR ){  // S_ISDIR() doesn't exist on my windows 
        printf( "%s is a directory\n", final_path_yolo );
    }else{
        printf( "%s is no directory\n", final_path_yolo );
        // is_path_exist=false;
    }
    return is_path_exist;
}

int The_Second_Filtering::Determine_Folder_Numb(string i_idx_numb){
    int temp_i;
    ifstream load_index;
    load_index.open(i_idx_numb);
    string index_str;
    if(load_index){
        getline(load_index, index_str);
        temp_i=stoi(index_str);
    }else{
        temp_i=0;
    }
    load_index.close();
    return temp_i;
}

void The_Second_Filtering::Splite_Data_Yolo_and_Contour_v3(string resource_file, string* yolo_data, string* contour_data, string* img_name ){
    
    string contour_line=resource_file;
    int delimiter = contour_line.find(';');
    string class_name = contour_line.substr(0,delimiter);//extract out class

    *yolo_data=class_name;
    *contour_data=class_name;

    *yolo_data=*yolo_data+' ';
    *contour_data=*contour_data+',';

    contour_line=contour_line.substr(delimiter+1);

    delimiter = contour_line.find(';');

    *img_name=contour_line.substr(0,delimiter);
    cout<<"*img_name: "<<*img_name<<endl;
    // cout<<"contour_line: "<<contour_line<<endl;
    // getchar();

    contour_line=contour_line.substr(delimiter+1);

    delimiter = contour_line.find(';');

    *contour_data=*contour_data+*img_name;
    *contour_data=*contour_data+',';
    // cout<<"img_name:"<<*img_name<<endl;
    // getchar();
    // *yolo_data=*yolo_data+"hello";

    

    string yo_info=contour_line.substr(0,delimiter);//extract out yolo information
    contour_line=contour_line.substr(delimiter+1);
    string contour_info=contour_line;

    *yolo_data=*yolo_data+yo_info;
    *contour_data=*contour_data+contour_info;
    // cout<<"*yolo_data: "<<*yolo_data<<endl;
}

void The_Second_Filtering::Read_Info_Contour_Yolo(){
    string contour_file="/contour_"+to_string(idx)+".txt";
    char contour_name_char[contour_file.length()+2];
    for(int j=0;j<sizeof(contour_name_char);j++){
        contour_name_char[j]=contour_file[j];
    }

    strcat(parent_path_contour,contour_name_char);


    // int parent_path_contour_size = sizeof(parent_path_contour) / sizeof(char); 
    // string myfile_path="";
    // for (int j=0; j<parent_path_contour_size; j++) { 
    //     myfile_path = myfile_path + parent_path_contour[j]; 
    // } 
    
    cout<<"sub_path_contour: "<<parent_path_contour<<endl;
    ifstream myfile;
    // string p="ROI_images/Contour/207/contour_207.txt";
    // myfile.open(parent_path_contour);
    myfile.open(parent_path_contour);
    // ifstream myfile( contour_name_char );
    // ifstream myfile( "ROI_images/Contour/200/contour_200.txt" );
    cout<<"parent_path_contour: "<<parent_path_contour<<endl;
    file_read=new string[glob_contour.gl_pathc];
    
    
    int ind=0;
    if(myfile){
        while(getline(myfile, file_read[ind])){
            // cout<<"file_read[ind]: "<<file_read[ind]<<endl;

            ind++;
        }
    }
    myfile.close();  
}
bool The_Second_Filtering::Digit_Check(string input_digit){
    int delimiter=0;
    while(delimiter!=-1){
        delimiter = input_digit.find(',');
        string temp_digit = input_digit.substr(0, delimiter);

        input_digit = input_digit.substr(delimiter+1);

        char digit_str[temp_digit.length()];
        for(int i=0;i<temp_digit.length();i++){
            char digit_str[1];
            digit_str[0]=temp_digit[i];
            if(isdigit(digit_str[0])==false){
                return false;
            }
        }
        
    }
    return true;
    
}

void The_Second_Filtering::Save_Info_of_Selected_Imgs(){
    string path_out_yolo =out_path_parent+"/yolo.txt";
    string path_out_contour=out_path_parent+"/contour.txt";
    string path_out_mix=out_path_parent+"/yolo_contour.txt";
    ofstream file_out_yolo;
    ofstream file_out_contour;
    ofstream file_out_mix;
    file_out_yolo.open(path_out_yolo,std::ios_base::app);//This is using for not overwrite
    file_out_contour.open(path_out_contour,std::ios_base::app);//This is using for not overwrite
    file_out_mix.open(path_out_mix,std::ios_base::app);//This is using for not overwrite

    int delimiter=0;
    while(delimiter!=-1){
        string yolo_data, contour_data, name_ori_img;

        delimiter = selected_idx.find(',');
        string temp_str = selected_idx.substr(0, delimiter);
        selected_idx = selected_idx.substr(delimiter+1);
        
        
        int temp_i=stoi(temp_str);       
        cout<<"file_read[temp_i]: "<<file_read[temp_i]<<endl;         
        
        Splite_Data_Yolo_and_Contour_v3(file_read[temp_i],&yolo_data, &contour_data, &name_ori_img);
        

        string temp_ori_name=glob_yolo.gl_pathv[temp_i];
        bool does_name_match=Name_Check(temp_ori_name,name_ori_img);
        if(does_name_match==true){
            cout<<"varified"<<endl;
        }else{
            cout<<"something wrong from name"<<endl;
            cout<<"temp_ori_name: "<<temp_ori_name<<endl;
            cout<<"name_ori_img: "<<name_ori_img<<endl;
            // return false;
        }

        string final_contour_path=out_path_contour+"/"+name_ori_img;
        // final_contour_path=final_contour_path+name_ori_img;

        string final_yolo_path=out_path_yolo+"/"+name_ori_img;
        // final_yolo_path=final_yolo_path+name_ori_img;
        


        imwrite(final_contour_path,image_contour[temp_i]);
        imwrite(final_yolo_path,image_original[temp_i]);
        
        // file_out<<file_read[temp_i]+"\n";
        // cout<<"yolo: "<<yolo_data<<endl;
        // cout<<"contour_data: "<<contour_data<<endl;
        file_out_yolo<<yolo_data;
        file_out_yolo<<endl;
        file_out_contour<<contour_data;
        file_out_contour<<endl;
        file_out_mix<<file_read[temp_i];
        file_out_mix<<endl;
        Save_Yolo(name_ori_img, yolo_data);

    }
    file_out_contour.close();
    file_out_yolo.close();
    file_out_mix.close();

    ofstream index_save;
    string idx_path=out_path_parent+"/index.txt";
    index_save.open(idx_path);
    index_save<<to_string(++idx);
    index_save.close();
}
void The_Second_Filtering::Save_Yolo(string img_name, string yolo_data){
    int delimiter=0;
    delimiter = img_name.find('.');
    img_name = img_name.substr(0, delimiter);
    img_name = img_name+".txt";
    ofstream yolo_save(out_path_yolo+img_name);
    yolo_save<<yolo_data;
    yolo_save<<endl;
    yolo_save.close();
}

bool The_Second_Filtering::Name_Check(string temp_ori_name, string name_ori_img){
    int delimiter=0;
    while(delimiter!=-1){
        delimiter = temp_ori_name.find('/');
        temp_ori_name =temp_ori_name.substr(delimiter+1);
    }
    cout<<"temp_ori_name: "<<temp_ori_name<<endl;
    if(temp_ori_name.compare(name_ori_img)==0){
        // cout<<"ori image is same"<<endl;
        return true;
    }else{
        // cout<<"ori image is different"<<endl;
        return false;
    }
}

bool The_Second_Filtering::Load_Img_and_Store_to_Mat(){

    glob(final_path_contour,GLOB_TILDE,NULL,&glob_contour);
    glob(final_path_yolo,GLOB_TILDE,NULL,&glob_yolo);

    the_number_of_files=glob_contour.gl_pathc-1;
    // if(the_number_of_files!=the_number_of_file_in_folder){//the_number_of_file_in_folder is in kuwahara.h
    //     return false;
    // }
    image_contour=new Mat[the_number_of_files];
    image_original=new Mat[the_number_of_files];
    
    Size size2(100,100);
    for(unsigned int j=0; j<the_number_of_files; j++){
        cout<<"glob_contour.gl_pathv[j]: "<<glob_contour.gl_pathv[j]<<endl;
        cout<<"glob_yolo.gl_pathv[j]: "<<glob_yolo.gl_pathv[j]<<endl;

        image_contour[j]=imread(glob_contour.gl_pathv[j],1);
        image_original[j]=imread(glob_yolo.gl_pathv[j],1);
        resize(image_contour[j],image_contour[j],size2);//resize image
        cv::putText(image_contour[j], //target image
        to_string(j), //text
        cv::Point(15, 15), //top-left position
        cv::FONT_HERSHEY_DUPLEX,
        0.4,
        CV_RGB(255, 0, 0), //font color
        1);
    }
    return true;
}

bool The_Second_Filtering::Load_Img_and_Store_to_Mat_V2(){

    glob(final_path_contour,GLOB_TILDE,NULL,&glob_contour);
    glob(final_path_yolo,GLOB_TILDE,NULL,&glob_yolo);

    the_number_of_files=glob_contour.gl_pathc-1;
    // cout<<"the_number_of_files: "<<the_number_of_files<<endl;
    // if(the_number_of_files!=the_number_of_file_in_folder){//the_number_of_file_in_folder is in kuwahara.h
    //     return false;
    // }
    image_contour=new Mat[the_number_of_files+1];
    image_original=new Mat[the_number_of_files+1];
    
    Size size2(100,100);
    int i=0;
    for(unsigned int j=0; j<the_number_of_files; j++){
        cout<<"glob_contour.gl_pathv[j]: "<<glob_contour.gl_pathv[j]<<endl;
        cout<<"glob_yolo.gl_pathv[j]: "<<glob_yolo.gl_pathv[j]<<endl;

        image_contour[j]=imread(glob_contour.gl_pathv[i],1);
        image_original[j]=imread(glob_yolo.gl_pathv[j],1);
        if(image_contour[j].empty()==true){
        	i++;
        	image_contour[j]=imread(glob_contour.gl_pathv[i],1);
        }
        resize(image_contour[j],image_contour[j],size2);//resize image
        cv::putText(image_contour[j], //target image
        to_string(j), //text
        cv::Point(15, 15), //top-left position
        cv::FONT_HERSHEY_DUPLEX,
        0.4,
        CV_RGB(255, 0, 0), //font color
        1);
        
        
        i++;
    }
    return true;
}

void The_Second_Filtering::Merge_Multi_Mats_to_Single_Mat_v2(Mat resource_mat[],int the_number_of_files){
    Size size2(100,100);

    Mat blank_mat=Mat::zeros(size2,CV_8UC3);

    int mat_size;
    if( (the_number_of_files%10)==0 ){//the case of 10,20,30...100
        mat_size=(the_number_of_files/10);
    }else{//the case of 11,12,13,14,21,22,23,etc..
        mat_size=(the_number_of_files/10)+1;
    }
    Mat **out=new Mat*[mat_size];
    for(int i = 0; i < mat_size; ++i){
        out[i] = new Mat[10];
    }
    int k=0;
    Mat *temp_out=new Mat[mat_size];
    for(int i=0;i<mat_size;i++){
        for(int j=0; j<10;j++){
            if( the_number_of_files%10!=0 && i==mat_size-1 ){// the case of 
                if(k<the_number_of_files){
                    out[i][j]=resource_mat[k];
                    k++;
                }else{
                    out[i][j]=blank_mat;    
                }
            }else{
                out[i][j]=resource_mat[k];
                k++;    
            }
        }
        // cout<<"i: "<<i<<endl;
        hconcat( out[i], 10, temp_out[i]);
    }
    Mat final_out;
    vconcat( temp_out, mat_size, final_out );
    for(int i = 0; i < mat_size; ++i){
        delete [] out[i];
    }
    delete [] out;
    delete [] temp_out;

    merged_img=final_out;
}

void Main_The_Second_Filtering(const string path_dataset,const string idx_value){
    // cout<<"path_dataset: "<<path_dataset<<endl;
    // cout<<"idx_value: "<<idx_value<<endl;
    
	string folder_name_root="output_the_second_contour_yolo/";
	string filename_parent=Extract_File_name_From_Full_Path(path_dataset);
	filename_parent=filename_parent+"/";
	string folder_name_contour="contour/";
    string folder_name_yolo="yolo/";
	Directory_Creater dc(folder_name_root, filename_parent, 
    	folder_name_contour, folder_name_yolo);

    string temp_parent_path=folder_name_root+filename_parent;
    string temp_contour_path=folder_name_root+filename_parent+folder_name_contour;
    string temp_yolo_path=folder_name_root+filename_parent+folder_name_yolo;

	The_Second_Filtering the_second_filtering(path_dataset, idx_value,
                                            temp_contour_path, temp_yolo_path, temp_parent_path);
	// cout<<"hello world"<<endl;
	the_second_filtering.Main_Process();
	// Directory_Creater write_file(the_number_of_file_in_folder,
	// 	folder_name_root,
	// 	 filename_parent,
	// 	 folder_name_roi,
	// 	 folder_name_contour,
	// 	  folder_name_ori_img);
	// bool are_files_reached_max_capacity=false;
}