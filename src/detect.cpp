#include "detect.h"

P_ML::P_ML(){
}

P_ML::~P_ML(){
}

int C_ML<ANN_MLP>::Predict(const Mat& data){
    float r = model->predict( data );
    int r_int=(int)round(r);//random forrest case
    return r_int;
}
int C_ML<Boost>::Predict(const Mat& data){
    float r = model->predict( data );
    int r_int=(int)round(r);//random forrest case
    return r_int;
}

int C_ML<RTrees>::Predict(const Mat& data){
    float r = model->predict( data );
    int r_int=(int)round(r);//random forrest case
    return r_int;
}

bool Load_Object_Label(const string path_contour_file, int& total_number_of_object, string*& name){
	ifstream tag_load;
    tag_load.open(path_contour_file);
    
    string read_line;
    int total;
    int i=0;
    if(tag_load.is_open()){
    	getline(tag_load, read_line);
    	total=stoi(read_line);//the first line is about total number of object.
    	total_number_of_object=stoi(read_line);
    	name= new string[total];

		while(getline(tag_load, read_line)){
			// cout<<"name: "<<name<<endl;
			name[i]=read_line;
			i++;
			if(i==total){break;}
		}
	}else{
		return false;
	}
	tag_load.close();
	return true;
}

Mat Print_Detected_Object(Mat input_img, Point object_location, int box_w, int box_h, int detected_class, string* class_name, int in_frame_w, int in_frame_h, int out_frame_w, int out_frame_h ){
	float generalized_label_location_x=(float)object_location.x/(float)in_frame_w;
	float generalized_label_location_y=(float)object_location.y/(float)in_frame_h;
	generalized_label_location_x=generalized_label_location_x*out_frame_w;
	generalized_label_location_y=generalized_label_location_y*out_frame_h;

	Point final_label_location;
	final_label_location.x=generalized_label_location_x;
	final_label_location.y=generalized_label_location_y;


	float generalized_label_width=(float)box_w/(float)in_frame_w;
	float generalized_label_height=(float)box_h/(float)in_frame_h;
	generalized_label_width=generalized_label_width*out_frame_w;
	generalized_label_height=generalized_label_height*out_frame_h;



	cv::putText(input_img, //target image
            class_name[detected_class], //text
           	final_label_location,
            FONT_HERSHEY_COMPLEX_SMALL, //top-left position
            1.0,
            CV_RGB(100*(detected_class+1), 70*(detected_class+1), 70*(detected_class)), //font color
            2);
	// cout<<"generalized_label_width: "<<generalized_label_width<<endl;
	// cout<<"generalized_label_height: "<<generalized_label_height<<endl;


	Rect rec(final_label_location.x, final_label_location.y, (int)generalized_label_width, (int)generalized_label_height);
	Point center_of_rect = (rec.br() + rec.tl())*0.5;

    circle(input_img,center_of_rect,1,Scalar(0,0,255));
	rectangle(input_img,rec,Scalar(0,255,0),2);



	return input_img;

}
// void C_ML<ANN_MLP>::Intialize(const Ptr<StatModel> model,const string path_weight){
// 	model=cv::ml::ANN_MLP::load(path_weight);
// }
// void C_ML<Boost>::Intialize(const Ptr<StatModel> model,const string path_weight){
// 	// model=cv::ml::Boost::load(path_weight);
// }
// void C_ML<RTrees>::Intialize(const Ptr<StatModel> model,const string path_weight){
// 	// model=cv::ml::RTrees::load(path_weight);
// }

int thresholding_image(Mat image, int value, bool inverted, int window_size){
	if(inverted==true){
		for(int x=(window_size/2); x<image.cols-(window_size/2);x++){
			for(int y=(window_size/2); y<image.rows-(window_size/2);y++){
				if((Mpixel(image,x,y)>=value)){
					Mpixel(image,x,y)=255;
				}else{
					Mpixel(image,x,y)=0;
				}
			}
		}	
	}else{
		for(int x=(window_size/2); x<image.cols-(window_size/2);x++){
			for(int y=(window_size/2); y<image.rows-(window_size/2);y++){				
				if(Mpixel(image,x,y)>=value){
					Mpixel(image,x,y)=0;
				}else{
					Mpixel(image,x,y)=255;
				}
			}
		}
	}
	int none_zero_pixel=0;
	if(inverted==true){
		for(int x=(window_size/2); x<image.cols-(window_size/2);x++){
			for(int y=(window_size/2); y<image.rows-(window_size/2);y++){
				if((int)Mpixel(image,x,y)>0){
					none_zero_pixel++;
				}
			}
		}	
	}else{
		for(int x=(window_size/2); x<image.cols-(window_size/2);x++){
			for(int y=(window_size/2); y<image.rows-(window_size/2);y++){				
				if((int)Mpixel(image,x,y)==0){
					none_zero_pixel++;
				}
			}
		}
	}
	return none_zero_pixel;
}
void Main_Detect(const string path_weight, const string path_tagfile, int ml_technique, const string source_video){
	
	double fps=0.0;
	VideoCapture cap(source_video);
	if(cap.isOpened()){
	    cout << "Video on" << endl;
	}else{
		cap.open(0);
		if (!cap.isOpened()){
		    cout << "Failed to open camera" << endl;
		    return;
		}
		cout << "Opened camera" << endl;
	}
	string* name;
	int total_class=2;//default value
	Load_Object_Label(path_tagfile, total_class, name);
	// for(int i=0;i<total_class;i++){
	// 	cout<<i<<": "<<name[i]<<endl;
	// }
	int cap_width=640;
	int cap_height=480;

	// int cap_width=800;
	// int cap_height=600;

	// int cap_width=640;
	// int cap_height=480;
	// int cap_width=320;
	// int cap_height=240;
	if(cap.isOpened()){
		cap.set(CAP_PROP_FRAME_WIDTH, cap_width);
		cap.set(CAP_PROP_FRAME_HEIGHT, cap_height);
	}
	// model=cv::ml::Boost::load(path_weight);
	P_ML *ml;
	if(ml_technique==0){
	    ml = Load_ML_With_Weight< C_ML <ANN_MLP> >(path_weight);
	}else if(ml_technique==1){
	    ml = Load_ML_With_Weight< C_ML<Boost> >(path_weight);
	}else if(ml_technique==2){
	    ml = Load_ML_With_Weight< C_ML<RTrees> >(path_weight);	   
	}else if (ml_technique==3){
		// ml = Load_ML_With_Weight< C_ML <SVM> >(path_weight);
	}else{
	    return; //errpr
	}

	/*The First image*/
	Mat image1;
	Mat gray_image1;
	int key=0;
	cap >> image1;
	if(!image1.data){printf("Could not open the file\n"); exit(0);}
	cvtColor(image1,gray_image1, COLOR_BGR2GRAY);//copy camera color image to gray scale

	Size frame_size(cap_width, cap_height);
    int frames_per_second = 20;

	VideoWriter oVideoWriter("MyVideo.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 
                                                               frames_per_second, frame_size, true);

	if(oVideoWriter.isOpened() == false) {
        cout << "Cannot save the video to a file" << endl;
        cin.get(); //wait for any key press
        return;
    }

    string window_name = "My Camera Feed";
    namedWindow(window_name); //create a window called "My Camera Feed"
    double pre0=0;
	double pre1=0;
	int detected_frame_numb=0;
	int total_fps=0;
	// cout<<"hhhhh";
	while (1){
		system_clock::time_point start = system_clock::now();//start clock

		Mat image2;
		cap >> image2;
		if(image2.empty()){
	    	break;
	    }
		// cout<<"image2.cols: "<<image2.cols<<endl;

		// cout<<"image2.rows: "<<image2.rows<<endl;
		// getchar();
		Mat output;
		Mat img1_g;
		Mat img2_g;
		cvtColor(image2, img2_g, COLOR_BGR2GRAY);//copy camera color image to gray scale
		cvtColor(image1, img1_g, COLOR_BGR2GRAY);//copy camera color image to gray scale

		output=img1_g-img2_g;
		int t_p=thresholding_image(output, 70,true,0);
		float percent=t_p/(float)(cap_width*cap_height);
		if(percent>0.0001){
			// image1=image2;
			while(1){
				
				system_clock::time_point start = system_clock::now();//start clock
				cap >> image2;
				if(image2.empty()){
	    			break;
	    		}
				Mat temp_img1;
				Mat temp_img2;

				temp_img1=image1.clone();
				temp_img2=image2.clone();
				
				// int temp_width=320;
				// int temp_height=240;
				// int temp_width=160;
				// int temp_height=120;

				// int temp_width=image2.cols/4;//default size
				// int temp_height=image2.rows/4;//default size

				// int temp_width=image2.cols;//default size
				// int temp_height=image2.rows;//default size

				int temp_width=image2.cols/4;//default size
				int temp_height=image2.rows/4;//default size

				// int temp_width=image2.cols/4;//default size
				// int temp_height=image2.rows/4;//default size

				// int temp_width=image2.cols/4;//default size
				// int temp_height=image2.rows/4;//default size

				// int temp_width=image2.cols/4;//default size
				// int temp_height=image2.rows/4;//default size


				resize(temp_img1, temp_img1, cv::Size(temp_width,temp_height));//resized
				resize(temp_img2, temp_img2, cv::Size(temp_width,temp_height));//resized

				Fourier_Descriptors FD;
				// Mat out=FD.Main_Detection_V2(image1,image2);
				Mat out=FD.Main_Detection_V2(temp_img1,temp_img2);

				
				int pre=ml->Predict(FD.final_CE_data);
				// cout<<"pre: "<<pre<<endl;
				if(pre==0){
					pre0++;
				}else{
					pre1++;
				}
				Mat out_with_label;				
				if(cap.isOpened()){
					out_with_label=Print_Detected_Object(image2, FD.label_location, FD.label_location_width,FD.label_location_height ,pre, name, temp_width, temp_height, image2.cols, image2.rows);
				}else{
					out_with_label=Print_Detected_Object(image2, FD.label_location, FD.label_location_width,FD.label_location_height ,pre, name, temp_width, temp_height, cap_width, cap_height);
				}
				// cout<<"pre: "<<pre<<endl;
				
				// bool isSuccess = cap.read(out_with_label); // read a new frame from the video camera
			 //    if (isSuccess == false){
		  //           cout << "Video camera is disconnected" << endl;
		  //           cin.get(); //Wait for any key press
		  //           // break;
		  //       }else{
		        oVideoWriter.write(out_with_label); 	
		        // }

				
	    		

			    Mat output1;
				Mat img1_g1;
				Mat img2_g1;
				cvtColor(image2, img2_g1, COLOR_BGR2GRAY);//copy camera color image to gray scale
				cvtColor(image1, img1_g1, COLOR_BGR2GRAY);//copy camera color image to gray scale

				output1=img1_g1-img2_g1;
				int t_p1=thresholding_image(output1, 70,true,0);
				float percent=t_p1/(float)(cap_width*cap_height);
				// cout<<"percent: "<<percent<<endl;
				// cout<<"total_pixel_percent:"<<percent<<endl;
	    		system_clock::time_point end = system_clock::now();
			    double seconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			    fps = 1000000/seconds;
			    cout << "frames " << fps << " seconds " << seconds << endl;
			    total_fps=total_fps+fps;

			    string speed_label="frames "+to_string(fps)+" seconds "+to_string(seconds);
			    Point p_speed_label;
			    p_speed_label.x=5;
			    p_speed_label.y=20;
			    cv::putText(out_with_label, //target image
				    speed_label, //text
				   	p_speed_label,
				    FONT_HERSHEY_COMPLEX_SMALL, //top-left position
				    1.0,
				    CV_RGB(100, 70, 70), //font color
				    1);

			    imshow("with_label" ,out_with_label);
				imshow("Analysing Window" ,out);
				key=waitKey(1);
				if(key==113 || key==27){
	    			break;
	    		}//either esc or 'q'
	    		

			    
		        

				// if(percent>0.03){
				// 	cout<<"break!!!"<<endl;
				// 	break;
				// }
				// if(percent<=0.007){
				// 	cout<<"break!!!"<<endl;
				// 	break;
				// }

	    		detected_frame_numb++;
			}	
			

		}
	
		// threshold(output, output, 0, 100, THRESH_BINARY | THRESH_OTSU);

		// cvtColor(image2,gray_image2, COLOR_BGR2GRAY);//copy camera color image to gray scale
		
		if(!image2.data){break;}
		// imshow("output" ,output);

	    	// imshow("image1" ,output1);

	    imshow("image2" ,image2);
	    key=waitKey(1);
	    	/*************/
	    	
	    	/*program termination*/
	    if(key==113 || key==27){
	    	break;
	    }//either esc or 'q'
	    image1=image2;
	    system_clock::time_point end = system_clock::now();
	    double seconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	    fps = 1000000/seconds;
	    cout << "frames " << fps << " seconds " << seconds << endl;
	    

	
	}

	float final_0=pre0/(pre0+pre1);
	float final_1=pre1/(pre0+pre1);
	cout<<"Average FPS:"<<(float)total_fps/(float)detected_frame_numb<<endl;
	cout<<"rodent percent="<<final_0<<endl;
	cout<<"bird percent="<<final_1<<endl;
	
}
