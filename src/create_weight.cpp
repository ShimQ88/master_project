#include "create_weight.h"

bool Read_Test_Contour_file(Mat *_data, int column_count, string file_name){
    ifstream read_CEs(file_name);
    Mat el_ptr(1, column_count, CV_32F);
    if(read_CEs.is_open()){
        _data->release();
        string line;
        vector<float>temp_CEs;
        while ( getline (read_CEs,line) ){
            // cout<<"_data: "<<_data<<endl;
            // cout<<"line: "<<line<<endl;
            int delimiter=0;
            int temp_i=0;
            delimiter = line.find(',');
            line = line.substr(delimiter+1);
            delimiter = line.find(',');
            line = line.substr(delimiter+1);
            while(true){
                delimiter = line.find(',');
                string temp_val=line.substr(0,delimiter);
                line = line.substr(delimiter+1);
                
                // cout<<"temp_val: "<<temp_val<<endl;
                // getchar();
                el_ptr.at<float>(temp_i)=stof(temp_val);
                temp_i++;
                if(delimiter==-1){break;}
            }
            cout<<"el_ptr: "<<el_ptr<<endl;
            _data->push_back(el_ptr);
            // cout<<"one line done"<<endl;
        }
        return true;
    }else{
        return false;
    }

}

string Name_Out(string input_file_name){
	int delimiter=0;
	string output_file_name;
	
	delimiter = input_file_name.find('.');
	string temp_name=input_file_name.substr(0,delimiter);
	output_file_name=temp_name+"_shuffled.txt";
	return output_file_name;
}

bool Read_Numb_Class_Data( const string& filename, int var_count,
                     Mat* _data, Mat* _responses, vector<string> *_imgname )
{
    
    ifstream final_output;
    final_output.open(filename);//output file
    
    string line;
    
    string replace_str=" ";



    Mat el_ptr(1, var_count-1, CV_32F);
    // string
    // _data->release();
    // Mat _data2(1000, var_count, CV_32F);
    // _data=*_data2;
    
    vector<int> responses;
    vector<string> imgname;

    _data->release();
    _responses->release();
    // _imgname->release();

    if(final_output.is_open()){
        string class_numb;
        string img_name;
        
        int j=0;
        
        while ( getline (final_output,line) ){
            Mat contour_info(1, var_count, CV_32F);
            int delimiter=0;
            int i=0;
            int temp_i=0;
            while(true){
                // cout<<"line: "<<line<<endl;
                delimiter = line.find(',');
                if(i==0){
                    class_numb = line.substr(0,delimiter);
                    responses.push_back( stoi(class_numb) );
                }else if(i==1){
                    img_name = line.substr(0,delimiter);
                    _imgname->push_back(img_name);
                }else{
                    string temp_contour;
                    temp_contour = line.substr(0,delimiter);
                    // _data[j][temp_i]= stof(temp_contour);
                    // _data->push_back(stof(temp_contour));
                    // Mpixel(_data,i,temp_i)=stof(temp_contour);
                    el_ptr.at<float>(temp_i)=stof(temp_contour);
                    temp_i++;
                }
                line=line.substr(delimiter+1);
                // cout<<"line: "<<line;
                if(delimiter==-1){break;}
                
                // line.replace(delimiter,1,replace_str);
                // cout<<"el_ptr: "<<el_ptr<<endl;
                // getchar();
                i++;
            }
            _data->push_back(el_ptr);
            // cout<<"class_numb: "<<class_numb<<endl;
            // cout<<"img_name: "<<img_name<<endl;
            // cout<<"_data: "<<_data[0]<<endl;

            // cout<<"contour_info[3]: "<<contour_info[3]<<endl;
            // cout<<"contour_info[4]: "<<contour_info[4]<<endl;
            // getchar();
            j++;
        }
    }

    Mat(responses).copyTo(*_responses);
    // Mat(imgname).copyTo(_imgname);

    return true;
}

int Count_Column_Numb_And_Shuffle_Contour_Data(const string& filename, string& path_shuffled_data){//only two case sample work need to develop more
    int numb_of_data_cols;
    ifstream myfile(filename);
    path_shuffled_data=Name_Out(filename);
    ofstream shuffled_file(path_shuffled_data);
    string line1[10000];
    string line2[10000];
    string temp_line;
    int i=0;
    int j=0;
    if (myfile.is_open())
    {
        // getline (myfile,line);  
        while ( getline (myfile,temp_line) )
        {
            if(i==0){
                numb_of_data_cols=count(temp_line.begin(), temp_line.end(), ',');    
            }
            
            if(temp_line[0]=='0')
                // if(temp_line[0]=='1')
            {
                // cout<<"hahaha"<<endl;
                line1[i]=temp_line;
                i++;
            }
            else if(temp_line[0]=='1')
                // else if(temp_line[0]=='9')
            {
                line2[j]=temp_line;
                j++;
            }
            else
            {
                cout<<"error"<<endl;
                getchar();
            }
            
        }
        
        // cout<<"numb_of_data_cols: "<<numb_of_data_cols<<endl;
        
        // cout<<"The first string: "<<temp_line[0];
        // getchar();
        myfile.close();
    }
    int i1=0;
    int j1=0;
    if (shuffled_file.is_open())
    {
        while(1){
            if( (i1==i)&&(j1==j) ){
                break;
            }
            if(i1!=i){
                shuffled_file <<line1[i1]+'\n';
                i1++;
            }
            if(j1!=j){
                shuffled_file << line2[j1]+'\n';
                j1++;
            }
            
        }
        shuffled_file.close();
    }
    
    return numb_of_data_cols;
}

void Main_Create_Weight(const string& filename_to_save,const string& filename_to_load, string path_contour_file, int the_number_of_class, string* path_all_class_dataset, float percent_of_division, int ml_technique){
	for(int i=0; i<the_number_of_class; i++){
		// cout<<i<<": "<<path_all_class_dataset[i]<<"ka"<<endl;
	}
    // cout<<"here?"<<endl;
	Mat data;
    Mat responses;
    string path_shuffled_data="";
    int numb_of_data_cols=Count_Column_Numb_And_Shuffle_Contour_Data(path_contour_file, path_shuffled_data);//shuffle the file
    // numb_of_data_cols=2;
    // cout<<"numb_of_data_cols: "<<numb_of_data_cols<<endl;
    // cout<<"path_shuffled_data: "<<path_shuffled_data<<endl;
    
    
    vector<string> imgname;
    bool ok = Read_Numb_Class_Data( path_shuffled_data, numb_of_data_cols, &data, &responses, &imgname);//third parameter: FEATURES
    // cout<<"ok: "<<ok<<endl;
    // cout<<"data: "<<data<<endl;
    // getchar();



    // cout<<"responses: "<<responses.at<int>(2,0)<<endl;
    // glob_t glob_result=Read_Dataset_Dir(path_all_class_dataset[i]);
    // ofstream ce_save;
    // ce_save.open("contour_CEs2.txt");
    

    // for(int i=0; i<imgname.size(); i++){
    //     int class_value=responses.at<int>(i,0);
    //     glob_t glob_result=Read_Dataset_Dir(path_all_class_dataset[class_value]);
    //     bool is_find=false;
    //     for(int j=0; j<glob_result.gl_pathc; j++){

            
    //         string name;
            
    //         int delimiter = imgname[i].find('.');
    //         name=imgname[i].substr(0,delimiter);
            
    //         string name1=name+".jpg";
    //         string name2=name+".JPG";
    //         // cout<<"name1:"<<name1<<endl;
    //         // cout<<"name2:"<<name2<<endl;
    //         string target_full_name1=path_all_class_dataset[class_value]+'/'+name1;
    //         string target_full_name2=path_all_class_dataset[class_value]+'/'+name2;
    //         string scan_name=glob_result.gl_pathv[j];
    //         if(target_full_name1.compare(scan_name)==0){
    //             is_find=true;
    //         }

    //         if(target_full_name2.compare(scan_name)==0){
    //             is_find=true;
    //         }
    //         if(is_find==true){
    //             Fourier_Descriptors FD;
    //             string target_file_name, prev_file_name, next_file_name;
    //             target_file_name=glob_result.gl_pathv[j];
    //             if(j==0){
    //                 prev_file_name="";
    //             }else{
    //                 prev_file_name=glob_result.gl_pathv[j-1];
    //             }

    //             if(j+1>=glob_result.gl_pathc){
    //                 next_file_name="";
    //             }else{
    //                 next_file_name=glob_result.gl_pathv[j+1];
    //             }

    //             bool is_success=FD.Main_The_First_Process(target_file_name, prev_file_name, next_file_name);

                
    //             string full_str=to_string(class_value)+','+imgname[i]+','+FD.contour_txt;
    //             cout<<"full_str: "<<full_str<<endl;
    //             ce_save<<full_str;
    //             ce_save<<endl;

                

                

    //             break;
    //         }
    //     }
    //     if(is_find==false){
    //         cout<<"serious error"<<endl;
    //         cout<<"the name: "<<path_all_class_dataset[class_value]+'/'+imgname[i]<<endl;
    //         getchar();
    //     }

    // }
    // ce_save.close();
    // // glob_t glob_result=Read_Dataset_Dir(path_dataset);
    // cout<<"done"<<endl;
    // return; 



    int nsamples_all = data.rows;
	
	int ntrain_samples=(int)round(nsamples_all*percent_of_division);//SPLIT
    int ntest_samples=(int)round(nsamples_all*(1-percent_of_division));//SPLIT


    //this process for dividing exactly with test value
    int remainder=ntrain_samples%ntest_samples;
    Mat CEs_data;
    bool is_success_contour_read=Read_Test_Contour_file(&CEs_data,numb_of_data_cols-2,"/home/kyu/Desktop/fd_yolo/contour_test.txt");
    // bool is_success_contour_read=Read_Test_Contour_file(&CEs_data,numb_of_data_cols-2,"/home/kyu/Desktop/fd_yolo/contour_test.txt");


    ntrain_samples=ntrain_samples-remainder;
    cout<<"ntrain_samples: "<<ntrain_samples<<endl;
    cout<<"ntest_samples: "<<ntest_samples<<endl; 

    //parameters in ANN
    int max_iter=50000;
    float method_param=0.00001;
    //parameters in Boost
    int boost_type=0;//Gentle 0.5 and true{DISCRETE, REAL, LOGIT, GENTLE}
    int weak_count=25;
    float weight_trim_rate=80.83;
    // float weight_trim_rate=20.83;
    // float weight_trim_rate=0.43;
    // int max_depth=5;
    //parameters in Random Forest Trees
    int max_depth=25;
    // int min_sample_count=10;
    int min_sample_count=10;
    float regression_accuracy=0.01f;
    int max_categories=2;
    int tc_value=700;
    int the_number_of_CEs=15;
    
    // cout<<"responses:" <<responses<<endl;
    // getchar();
    // cout<<"data:" <<data<<endl;
    // getchar();

    Machine_Learning_Data_Preparation *prepared_data = new Machine_Learning_Data_Preparation(data, responses, CEs_data, imgname, path_all_class_dataset, ntrain_samples, ntest_samples, filename_to_save, filename_to_load, the_number_of_class, the_number_of_CEs);
    
    prepared_data->Main_Process(ml_technique);//data arrangement
    
    cout<<"Training Process On....";
    Parent_ML *final_ml;
    if(ml_technique==0){
        final_ml = Creat_ML_Class< Child_ML <ANN_MLP> >(max_iter,0,method_param,0,0);//{max_iter,null,method_param,null,null}
        // prepared_data->Main_Process2("weight/ANN.xml", ml_technique,max_iter,0,method_param,0,0);
    }else if(ml_technique==1){
        final_ml = Creat_ML_Class< Child_ML <Boost> >(boost_type,weak_count,weight_trim_rate,max_depth,0);
        // prepared_data->Main_Process2("weight/Boost.xml", ml_technique,boost_type,weak_count,weight_trim_rate,max_depth,0);
    }else if(ml_technique==2){
        // final_ml = Creat_ML_Class< Child_ML <RTrees> >(max_depth,min_sample_count,regression_accuracy,max_categories,tc_value);
        prepared_data->Main_Process2("weight/rf_demo.xml", ml_technique,max_depth,min_sample_count,regression_accuracy,max_categories,tc_value);
        // final_ml = Creat_ML_Class< Child_ML <SVM> >(max_depth,min_sample_count,regression_accuracy,max_categories,tc_value);
    }else if(ml_technique==3){
        final_ml = Creat_ML_Class< Child_ML <SVM> >(max_depth,min_sample_count,regression_accuracy,max_categories,tc_value);
        // prepared_data->Main_Process2("weight/SVM.xml", ml_technique,min_sample_count,regression_accuracy,max_categories,max_depth,0);

    }else{


        // cout<<"ml_technique code error"<<endl;
        // return false;
    }
    // cout<<"here"<<endl;
    return;
    // getchar();
    final_ml->Main_Process(prepared_data);//doing main process
    Write_File file_write(final_ml, prepared_data, to_string(min_sample_count));//writing file class
    file_write.Write_Header(final_ml->Head_Parameter());
    file_write.Main_Process();
    // prepared_data->Main_Process2(ml_technique,);
    // final_ml = Creat_ML_Class< Child_ML <RTrees> >(max_depth,min_sample_count,regression_accuracy,max_categories,tc_value);
    // cout<<"doone"<<endl;
    // final_ml->Create_Weight("hello");
    // cout<<"good3"<<endl;
    // cout<<"data[10]: "<<data<<endl;
    // cout<<"responses[10]: "<<responses<<endl;
    // cout<<"imgname[1]: "<<imgname<<endl;
    // cout<<"img_root: "<<img_root<<endl;
    // string img_full_path=img_root+"/"+imgname[20];
    // cout<<"img_full_path: "<<img_full_path<<endl;
}
