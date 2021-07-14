#include "main.h"

void info(){
    cout<<"hello this is the program for machine learning"<<endl;
    cout<<"choose which technique you want to test for it"<<endl;
    cout<<"mode 0: the first process"<<endl;
    cout<<"mode 1: the second process"<<endl;
    cout<<"mode 2: the last process"<<endl;
    cout<<"command 0: ./main -mode 0 --cls <class_number> --data <path_dataset> --idx <path_index_file>"<<endl;
    cout<<"command 1: ./main -mode 1 --data <path_dataset> --idx <path_index_file>"<<endl;
    cout<<"command 2: ./main -mode 2 --data <path_dataset> --idx <path_index_file>"<<endl;
    cout<<"command 3: ./main -mode 3 --data <path of contour file> --cls_root <directories of dataset> --save <directory to save weight> --load <directory to load weight>--per <percent_of_division_of_test_and_train> --tech <0:CNN, 1:ADA_Boost, 2:Random_Forest>"<<endl;
    cout<<"command 4: ./main -mode 4 --cls 0 --data <path_dataset>,<path_dataset> --contour contour.txt"<<endl;
    // ./main -mode 3 --data contour.txt --cls_/home/kyu/Desktop/rodents_data, /home/kyu/Desktop/birds
    // ./main -mode 4 --cls 0 --contour contour.txt
    // cout<<"1 = ada_boost"<<endl;
    // cout<<"2 = random_forest"<<endl;
    // cout<<"-detect"<<endl;
}

int function2(string paths_dataset, string *&out_data){
	
	int delimiter=0;
	int the_number_of_file=0;
	cout<<"paths_dataset: "<<paths_dataset<<endl;
	string temp_path=paths_dataset;
	while(delimiter!=-1){//the process how many files are there.
		delimiter = temp_path.find(',');
		temp_path=temp_path.substr(delimiter+1);

		the_number_of_file++;
	}
	cout<<"the_number_of_file: "<<the_number_of_file<<endl;

	out_data=new string[the_number_of_file];

	delimiter=0;
	int i=0;
	while(delimiter!=-1){//the process how many files are there.
		delimiter = paths_dataset.find(',');
		
		temp_path=paths_dataset.substr(0,delimiter);
		out_data[i]=temp_path;

		paths_dataset=paths_dataset.substr(delimiter+1);
		i++;

	}
	// cout<<"out_data[0]:"<<out_data[0]<<endl;
	// cout<<"out_data[1]:"<<out_data[1]<<endl;
	// cout<<"out_data[2]:"<<out_data[2]<<endl;
	// cout<<"out_data[3]:"<<out_data[3]<<endl;
	// getchar();
    return the_number_of_file;
}


//this function using for reading the index from file
string Read_Index_From_File(const string& path_index_file){
	ifstream file_read_index;
	string output_idx;
    file_read_index.open(path_index_file);
    if(file_read_index.is_open()){//success to read
    	string temp_line;
    	getline(file_read_index,temp_line);
    	output_idx=temp_line;
    }else{//fail to read file
    	  //return default value as zero
    	output_idx="0";

    }
    return output_idx;
}


int main(int argc, char *argv[]){
	info();

	if(strcmp(argv[1],"-mode")==0){
		//default valiable
		string class_number="0";
		string path_dataset="error";
		string idx_value="0";
		string *path_all_class_dataset;
		string path_save="";
		string path_load="";
		string percent_of_division="0.8";
		string training_technique="2";//Random Forest
		string path_contour_file;
		string path_yolov5;
		string save_location="";
		string path_txt="";
		string path_weight="";
		string path_tagfile="";
		string source_video="";

		int the_number_of_class=0;

		for(int i=3; i<argc; i++){//process collecting data
			if(strcmp(argv[2],"0")==0){//function1
				
				if(strcmp(argv[i],"--cls")==0){
					class_number=argv[i+1];
				}

				if(strcmp(argv[i],"--data")==0){
					path_dataset=argv[i+1];
				}

				if(strcmp(argv[i],"--idx")==0){
					idx_value=Read_Index_From_File(argv[i+1]);
				}


			}else if(strcmp(argv[2],"1")==0){//function1

				if(strcmp(argv[i],"--data")==0){
					path_dataset=argv[i+1];
				}

				if(strcmp(argv[i],"--idx")==0){
					idx_value=Read_Index_From_File(argv[i+1]);
				}



			}else if(strcmp(argv[2],"2")==0){//function2

				if(strcmp(argv[i],"--data")==0){
					path_dataset=argv[i+1];
				}

				if(strcmp(argv[i],"--idx")==0){
					idx_value=Read_Index_From_File(argv[i+1]);
				}
				
			}else if(strcmp(argv[2],"3")==0){//function3
				if(strcmp(argv[i],"--data")==0){
					path_contour_file=argv[i+1];
				}
				if(strcmp(argv[i],"--cls")==0){
					the_number_of_class=stoi(argv[i+1]);
					
				}

				if(strcmp(argv[i],"--cls_root")==0){
					cout<<"function here"<<endl;
					cout<<"argv[i+1]: "<<argv[i+1]<<endl;
					function2(argv[i+1], path_all_class_dataset);
					cout<<"function done"<<endl;
					
				}

				if(strcmp(argv[i],"--save")==0){
					path_save=argv[i+1];
				}

				if(strcmp(argv[i],"--load")==0){
					path_load=argv[i+1];
				}

				if(strcmp(argv[i],"--per")==0){
					percent_of_division=argv[i+1];
				}

				if(strcmp(argv[i],"--tech")==0){
					training_technique=argv[i+1];
				}
			}else if(strcmp(argv[2],"4")==0){//function4

				if(strcmp(argv[i],"--cls")==0){
					class_number=argv[i+1];
				}

				if(strcmp(argv[i],"--data")==0){
					path_dataset=argv[i+1];
				}

				if(strcmp(argv[i],"--idx")==0){
					idx_value=Read_Index_From_File(argv[i+1]);
				}

				if(strcmp(argv[i],"--contour")==0){
					path_contour_file=argv[i+1];
				}
				
				if(strcmp(argv[i],"--path_yolov5")==0){
					path_yolov5=argv[i+1];
				}

				if(strcmp(argv[i],"--save")==0){
					save_location=argv[i+1];
				}
			}else if(strcmp(argv[2],"5")==0){//function4

				if(strcmp(argv[i],"--cls")==0){
					class_number=argv[i+1];
				}

				if(strcmp(argv[i],"--data")==0){
					path_dataset=argv[i+1];
				}

				if(strcmp(argv[i],"--yolo_txt")==0){
					path_txt=argv[i+1];
				}

				if(strcmp(argv[i],"--idx")==0){
					idx_value=Read_Index_From_File(argv[i+1]);
				}

				if(strcmp(argv[i],"--contour")==0){
					path_contour_file=argv[i+1];
				}
				
				if(strcmp(argv[i],"--path_yolov5")==0){
					path_yolov5=argv[i+1];
				}

				if(strcmp(argv[i],"--save")==0){
					save_location=argv[i+1];
				}
			}else if(strcmp(argv[2],"6")==0){//function4

				if(strcmp(argv[i],"--weight")==0){
					path_weight=argv[i+1];
				}

				if(strcmp(argv[i],"--tech")==0){
					training_technique=argv[i+1];
				}

				if(strcmp(argv[i],"--tag")==0){
					path_tagfile=argv[i+1];
				}

				if(strcmp(argv[i],"--source")==0){
					source_video=argv[i+1];
				}
			}
		}

		if(strcmp(argv[2],"0")==0){
			Main_Contour_Yolo_Creater(class_number, path_dataset, idx_value);

		}else if(strcmp(argv[2],"1")==0){
			Main_The_Second_Filtering(path_dataset, idx_value);

		}else if(strcmp(argv[2],"2")==0){
			Main_The_Final_Filtering(path_dataset, idx_value);
			
		}else if(strcmp(argv[2],"3")==0){
			Main_Create_Weight(path_load, path_save, path_contour_file, the_number_of_class,path_all_class_dataset,stof(percent_of_division), stoi(training_technique));
			delete [] path_all_class_dataset;
			// load_and_save_ml( const string& data_filename, const string& img_root,
   //                    const string& filename_to_save,
   //                    const string& filename_to_load,
   //                    float percent_of_division,
   //                    int ml_technique)
		}else if(strcmp(argv[2],"4")==0){
			Main_Contour_Yolo_Creater_With_Window_Info(class_number, path_dataset, idx_value, path_contour_file, path_yolov5, save_location);
		}else if(strcmp(argv[2],"5")==0){
			Main_Contour_Yolo_Creater_With_Window_Info_Video(class_number, path_dataset, path_txt, idx_value, path_contour_file, path_yolov5, save_location);

		}else if(strcmp(argv[2],"6")==0){
			Main_Detect(path_weight, path_tagfile,stoi(training_technique),source_video);
		}





		// load_and_save_ml( const string& data_filename, const string& img_root,
  //                     const string& filename_to_save,
  //                     const string& filename_to_load,
  //                     float percent_of_division,
  //                     int ml_technique)
	}else{
		cout<<"Command error -mode must be located after main"<<endl;
		exit(2);
	}
	
}