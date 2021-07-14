#include "contour_yolo_creater.h"
#include "detect.h"

bool Check_Numb_In_A_Row(string prev_name, string target_name){
	string str_prev_name = prev_name.substr(prev_name.size()-8,4);
	string str_target_name = target_name.substr(target_name.size()-8,4);
	//temperay change
	// string str_prev_name = prev_name.substr(prev_name.size()-7,3);
	// string str_target_name = target_name.substr(target_name.size()-7,3);

	int prev_img_numb = stoi(str_prev_name);
	int target_img_numb = stoi(str_target_name);

	int diff_numb = target_img_numb-prev_img_numb;
	
	if( (diff_numb==1)||(diff_numb==-1) ){
		return true;
	}else{
		return false;
	}

}


//extra functions
string Extract_File_name_From_Full_Path(string name){
	int delimiter=0;
	string output_file_name;
	while(delimiter!=-1){
		delimiter = name.find('/');
		name=name.substr(delimiter+1);
	}
	output_file_name=name;
	return output_file_name;
}

glob_t Read_Dataset_Dir(string temp_path){
	glob_t output_glob;
	char path[temp_path.size()+2];//if could not read change the number of 10
	temp_path=temp_path+"/*";
	// cout<<"temp_path[i]:";
	for(int i=0;i<temp_path.size()+2;i++){
		
		path[i]=temp_path[i];
	}
	
	glob(path,GLOB_TILDE,NULL,&output_glob);
	return output_glob;
}

bool Index_Saver(string path,unsigned int i){
	//index save area
	ofstream index_save;
	index_save.open (path);
	index_save << to_string(i);
	index_save.close();
	return true;
}



void Median_Filter(Mat image1,Mat image2,int window_size){
    //picture elements(pels)
    int function_size_input=window_size;//this is for window size

    int picture_x_size=image1.cols;
    int picture_y_size=image1.rows;
    int mdn;//median value in a window
    int ltmdn=0;// number of pels having gray levels less than mdn in a window
    int window_x_size=function_size_input;
    int window_y_size=function_size_input;
    //int hist[window_x_size*window_y_size];
    int index_for_array=0;
    int count_of_less_than_median=0;
    int median_value;
    int left_column[window_y_size];
    int right_column[window_y_size];
    // int left_column[window_y_size-1];
    // int right_column[window_y_size-1];
    int hist_index=0;
    int g1;

    int th=(window_x_size*window_y_size/2);
    
    // for(int i=0;i<picture_y_size;i++){
    //      Mpixel(image2,637,i)=255;
    //  }
    
    for(int i=(window_x_size/2);i<picture_x_size-(window_x_size/2);i++){
        
        int hist[256];
        for(int q=0;q<256;q++){
            hist[q]=0;
        }

        int index_for_hist=0;
        int y_sizeof=i+window_y_size;
        int x_sizeof=(window_x_size/2)+window_x_size;
        for(int a=i;a<y_sizeof;a++){
            for(int b=(window_x_size/2);b<x_sizeof;b++){
                index_for_hist=(int)Mpixel(image1,a,b);
                hist[index_for_hist]=hist[index_for_hist]+1;
            }
        }


        int counter_for_find_median=(window_x_size*window_y_size/2)+1;
        int counter_for_find_less_than_median=0;

        for(int z=0;z<256;z++){
            if(hist[z]!=0){ 
                counter_for_find_median=counter_for_find_median-hist[z];
                if(counter_for_find_median<=0){
                    median_value=z;
                    mdn=median_value;
                    break;
                }else{
                    counter_for_find_less_than_median
                    =counter_for_find_less_than_median+hist[z];
                }
            }       
        }

        ltmdn=counter_for_find_less_than_median;

        //Mpixel(image2,i,(window_y_size/2))=mdn;

        for(int j=(window_y_size/2)+1;j<picture_y_size-(window_y_size/2);j++){//j indicates picture column number

    
            int index_for_left_column=0;
            int index_for_right_column=0;

            for(int l=i;l<i+window_x_size;l++){
                left_column[index_for_left_column]=(int)Mpixel(image1,l,j);
                index_for_left_column++;

                right_column[index_for_right_column]=(int)Mpixel(image1,l,j+window_x_size-1);               
                index_for_right_column++;   
            }
            
                    
            

            for(int k=0;k<window_y_size;k++){
                g1=left_column[k];
                hist[g1]=hist[g1]-1;
                if(g1<mdn){
                    ltmdn=ltmdn-1;
                }
                g1=right_column[k];
                hist[g1]=hist[g1]+1;
                if(g1<mdn){
                    ltmdn=ltmdn+1;
                }

                if(ltmdn>th){
                    while(true){
                        mdn=mdn-1;
                        ltmdn=ltmdn-hist[mdn];
                        if(ltmdn<=th){
                            break;
                        }
                    }
                }else{
                    while(ltmdn+hist[mdn]<=th){
                        ltmdn=ltmdn+hist[mdn];
                        mdn=mdn+1;
                    }
                }
            }
            Mpixel(image2,i,j)=mdn; //determine pixel
        }
    }

    for(int j=0;j<window_size/2+1;j++){ 
        for(int i=0;i<picture_y_size;i++){
            Mpixel(image2,j,i)=0;
            Mpixel(image2,picture_x_size-1-j,i)=0;
        }
    }
    for(int j=0;j<window_size/2+1;j++){ 
        for(int i=window_size/2;i<picture_x_size-(window_size/2);i++){
            Mpixel(image2,i,j)=0;
            Mpixel(image2,i,picture_y_size-1-j)=0;
        }
    }

}


void Fourier_Descriptors::EllipticFourierDescriptors(std::vector<Point>& contour, std::vector<float> &CE){
	vector<float>ax,ay,bx,by;
	int m=contour.size();
	int n=n_CE;//number of CEs we are interested in computing
	float t=(2*PI)/m;
	for(int k=0;k<n;k++){
		ax.push_back(0.0);
		ay.push_back(0.0);
		bx.push_back(0.0);
		by.push_back(0.0);
		for(int i=0;i<m;i++){
			ax[k]=ax[k]+contour[i].x*cos((k+1)*t*(i));
			bx[k]=bx[k]+contour[i].x*sin((k+1)*t*(i));
			ay[k]=ay[k]+contour[i].y*cos((k+1)*t*(i));
			by[k]=by[k]+contour[i].y*sin((k+1)*t*(i));
		}
	ax[k]=(ax[k])/m;
	bx[k]=(bx[k])/m;
	ay[k]=(ay[k])/m;
	by[k]=(by[k])/m;
	}
	for(int k=0; k<n; k++){
		CE.push_back(sqrt((ax[k]*ax[k]+ay[k]*ay[k])/(ax[0]*ax[0]+ay[0]*ay[0]))+sqrt((bx[k]*bx[k]+by[k]*by[k])/(bx[0]*bx[0]+by[0]*by[0])));
	}
	for(int count=0; count<n&&count<CE.size(); count++){
		// printf("%d CE %f ax %f ay %f bx %f by %f\n",count,CE[count],ax[count],ay[count],bx[count],by[count]);
	}
	// getchar();
}

int Fourier_Descriptors::Distance_Between_Points(Point p1, Point p2){
	int distance_x = (p2.x - p1.x)*(p2.x - p1.x);
  	int distance_y = (p2.y - p1.y)*(p2.y - p1.y);
  	return sqrt(distance_x + distance_y);
}

int Fourier_Descriptors::Find_The_Object_Contour(std::vector<vector<Point>>contours,Point center_of_object){
	int g_val=9999;
	int final_i=-1;
	for(int i=0;i<contours.size();i++){
		int l_val=9999;
		for(int j=0;j<contours[i].size();j++){
			int distance=Distance_Between_Points(contours[i][j],center_of_object);
			if(l_val>distance){
				l_val=distance;
			}
	
		}
		if(g_val>l_val){
			g_val=l_val;
			final_i=i;
		}
	}
	return final_i;
}

int Fourier_Descriptors::Determine_Frame_Size(Mat input, Point obj_center, int frame_size, int append_size ){
	bool is_tri=false;
	// cout<<"input.size(): "<<input.size()<<endl;
	// cout<<"obj_center.x: "<<obj_center.x<<endl;
	// cout<<"obj_center.y: "<<obj_center.y<<endl;

 	if( (obj_center.x-frame_size < 0) || (obj_center.y-frame_size < 0 ) ){//the case1 of out of frame boundary
 		// cout<<"return case1"<<endl;
 		// return -1;
 		return frame_size*2;
	}

	if( (obj_center.x+frame_size > input.cols) || (obj_center.y+frame_size > input.rows) ){//the case2 of out of frame boundary
		// cout<<"return case2"<<endl;
 		// return -2;
 		return frame_size*2;
	}

	while(true){
		if( (obj_center.x-frame_size<=0)||(obj_center.y-frame_size<=0) ){//The frame size bigger than the left and top side margin so finish expanding more frame size
			// cout<<"return case3"<<endl;
			return frame_size*2;
		}

		if( (obj_center.x+frame_size >= input.cols)||(obj_center.y+frame_size >= input.rows) ){//The frame size bigger than the left and top side margin so finish expanding more frame size
			// cout<<"return case4"<<endl;
			return frame_size*2;
		}

		for(int k=obj_center.x-frame_size;k<obj_center.x+frame_size;k++){			
			for(int l=obj_center.y-frame_size;l<obj_center.y+frame_size;l++){

				// cout<<"k: "<<k<<endl;
				// cout<<"l: "<<l<<endl;
				// cout<<"obj_center.x: "<<obj_center.x<<endl;
				// cout<<"obj_center.y: "<<obj_center.y<<endl;
				if( (k==obj_center.x-frame_size)||(k==obj_center.x+frame_size-1)||(l==obj_center.y-frame_size)||(l==obj_center.y+frame_size-1) ){
					// Point t_pp;
					// t_pp.x=k;
					// t_pp.y=l;
					// circle(subtracted_frame,t_pp,3,Scalar(255,255,255));
					if((int)Mpixel(input,k,l)!=0 ){
						is_tri=true;
					}
				}


			}	
		}

		
		
		frame_size=frame_size+append_size;

		// cout<<"frame_size: "<<frame_size<<endl;
		if(is_tri==false){
			// cout<<"normal case: "<<endl;
			// cout<<"frame_size: "<<frame_size*2<<endl;
			break;
		}
		is_tri=false;

		// if(frame_size>=250){//maximum size
		if(frame_size>=250){//maximum size this is for testing
			return frame_size*2;
			cout<<"error for size"<<endl;
		}
	}
	return frame_size*2;
}

Point Fourier_Descriptors::Draw_Rect_Box(Mat input_image, Point* p1, Point* p2, int loop_number){
	int big_blob_i = -1;
	int big_blob_len = -9999;
	for(int i=0;i<loop_number;i++){
		if((p1[i].x!=9999)&&(p1[i].y!=9999)&&(p2[i].x!=-9999)&&(p2[i].y!=-9999)){
			int temp_value = abs( (p2[i].x-p1[i].x)*(p2[i].y-p1[i].y) );
			if(big_blob_len < temp_value){
				big_blob_len= temp_value;
				big_blob_i = i;
			}
		}
	}
	Rect rec(p1[big_blob_i].x-2, p1[big_blob_i].y-2, p2[big_blob_i].x-p1[big_blob_i].x+2, p2[big_blob_i].y-p1[big_blob_i].y+2);
	
	Point center_of_rect = (rec.br() + rec.tl())*0.5;

 //    circle(input_image,center_of_rect,1,Scalar(0,0,255));
	// rectangle(input_image,rec,Scalar(0,255,0),2);
	// cout<<"centre: "<<center_of_rect.x<<", "<<center_of_rect.y<<endl;
	// cout<<"p1: "<<p1[big_blob_i].x<<", "<<p1[big_blob_i].y<<endl;
	// cout<<"p2: "<<p2[big_blob_i].x<<", "<<p2[big_blob_i].y<<endl;

	return center_of_rect;
}
Point Fourier_Descriptors::Draw_Rect_Box2(Mat input_image, Point* p1, Point* p2, int loop_number){
	int big_blob_i = -1;
	int big_blob_len = -9999;
	for(int i=0;i<loop_number;i++){
		if((p1[i].x!=9999)&&(p1[i].y!=9999)&&(p2[i].x!=-9999)&&(p2[i].y!=-9999)){
			int temp_value = abs( (p2[i].x-p1[i].x)*(p2[i].y-p1[i].y) );
			if(big_blob_len < temp_value){
				big_blob_len= temp_value;
				big_blob_i = i;
			}
		}
	}
	Rect rec(p1[big_blob_i].x-2, p1[big_blob_i].y-2, p2[big_blob_i].x-p1[big_blob_i].x+2, p2[big_blob_i].y-p1[big_blob_i].y+2);
	
	label_location.x=p1[big_blob_i].x-2;//label x location for writing 
	label_location.y=p1[big_blob_i].y-2;//label y location for writing
	label_location_width=p2[big_blob_i].x-p1[big_blob_i].x+2;
	label_location_height=p2[big_blob_i].y-p1[big_blob_i].y+2;

	// cout<<"p2[big_blob_i].x-p1[big_blob_i].x+2: "<<p2[big_blob_i].x-p1[big_blob_i].x+2<<endl;
	// cout<<"p2[big_blob_i].y-p1[big_blob_i].y+2: "<<p2[big_blob_i].y-p1[big_blob_i].y+2<<endl;


	Point center_of_rect = (rec.br() + rec.tl())*0.5;

    circle(input_image,center_of_rect,1,Scalar(0,0,255));
	rectangle(input_image,rec,Scalar(0,255,0),2);
	// cout<<"centre: "<<center_of_rect.x<<", "<<center_of_rect.y<<endl;
	// cout<<"p1: "<<p1[big_blob_i].x<<", "<<p1[big_blob_i].y<<endl;
	// cout<<"p2: "<<p2[big_blob_i].x<<", "<<p2[big_blob_i].y<<endl;

	return center_of_rect;
}

int Fourier_Descriptors::Blob(Mat image2, Mat image3,Point *p1_rec, Point *p2_rec){

	int counterColour=0;

	// cout<<"image2.cols:"<<image2.cols<<endl;
	// cout<<"image2.rows:"<<image2.rows<<endl;
	// getchar();
	vector< tuple<int, int> > SET[image2.cols];
	// vector< tuple<int, int> > SET[2000];
	
	int A[image2.cols][image2.rows];

	
	int s1,s2;

	for(int x=0; x<image2.cols;x++){
		for(int y=0; y<image2.rows;y++){
			A[x][y]=-1;
		}
	}
	// cout<<"P1"<<endl;
	int i=0;
	int index=0, counter=-1;
	bool is_s1_in=false;
	// for(int y=1; y<image2.rows;y++){
	for(int y=image2.rows/50; y<image2.rows;y++){//Ignoring the numbers from top section
		for(int x=1; x<image2.cols;x++){
			// cout<<"counter: "<<counter<<endl;
			if(Mpixel(image2,x,y)!=0){
				if((Mpixel(image2,x-1,y)!=0)||(Mpixel(image2,x,y-1)!=0)){
					s1=A[x-1][y];
					s2=A[x][y-1];
					if(s1!=-1){
						SET[s1].push_back(make_tuple(x,y));
						
						A[x][y]=s1;
						is_s1_in=true;
					}
					if(s2!=-1){
						SET[s2].push_back(make_tuple(x,y));
						A[x][y]=s2;
						is_s1_in=false;

					}
					if((s1!=s2)&&(s1!=-1)&&(s2!=-1)){
						SET[s1].insert(SET[s1].end(),SET[s2].begin(),SET[s2].end());						
						for (int j = 0; j < SET[s2].size(); j++ ){
							A[get<0>(SET[s2][j])][get<1>(SET[s2][j])]=s1;
						}				
						SET[s2].clear();
					}
					is_s1_in=false;

				}else{
					counter++;
					SET[counter].push_back(make_tuple(x,y));
					A[x][y]=counter;
				}

			}

			// if(counter>150){return -100;}//This is using for avoiding segmental fault.
			if(counter>150){break;}//This is using for avoiding segmental fault.
		}


	}

	// cout<<"P2"<<endl;
	//colour counting
	// for(int i=0;i<image2.cols;i++){
	// 	colour_count(SET[i]);
	// }

	//put random colour to image3
	
	counter++;
	
	

	// Point p1_rec[counter];
	// Point p2_rec[counter];

	// p1_rec= new Point[counter];
	// p2_rec= new Point[counter];

	for(int i=0;i<counter;i++){
		p1_rec[i].x=9999;
		p1_rec[i].y=9999;

		p2_rec[i].x=-9999;
		p2_rec[i].y=-9999;
	}
	
	// cout<<"P3"<<endl;

	int rand_numb1 = rand() % 100; 
	int rand_numb2 = rand() % 100; 
	int rand_numb3 = rand() % 100;

	for(int y=image2.rows/50; y<image2.rows;y++){//Ignoring the numbers from top section
		for(int x=0; x<image2.cols;x++){
			if(y==0){
				if(SET[x].empty()==true){

				}else{
					counterColour++;
				}
				// colour_count(SET[x]);
			}
			if(A[x][y]==-1){
				pixelB(image3,x,y)=0;
				pixelG(image3,x,y)=0;
				pixelR(image3,x,y)=0;	
			}
			if(A[x][y]!=-1){	
				pixelB(image3,x,y)=(A[x][y]+1)*rand_numb1;
				pixelG(image3,x,y)=(A[x][y]+1)*rand_numb2;
				pixelR(image3,x,y)=(A[x][y]+1)*rand_numb3;

				if(x < p1_rec[ (A[x][y]) ].x){
					p1_rec[ (A[x][y]) ].x=x;
				}

				if(y < p1_rec[ (A[x][y]) ].y){
					p1_rec[ (A[x][y]) ].y=y;
				}


				if(x > p2_rec[ (A[x][y]) ].x){
					p2_rec[ (A[x][y]) ].x=x;
				}

				if(y > p2_rec[ (A[x][y]) ].y){
					p2_rec[ (A[x][y]) ].y=y;
				}

			}
		}
	}
	Draw_Rect_Box(image3, p1_rec, p2_rec, counter);
	// cout<<"P4"<<endl;
	// for(int i=0;i<counter;i++){
	// 	if((p1_rec[i].x!=9999)&&(p1_rec[i].y!=9999)&&(p2_rec[i].x!=-9999)&&(p2_rec[i].y!=-9999)){
	// 		Rect rec(p1_rec[i].x, p1_rec[i].y, p2_rec[i].x-p1_rec[i].x+1, p2_rec[i].y-p1_rec[i].y+1);
	// 		rectangle(image3,rec,Scalar(0,255,0),1);
	// 	}
	// }
	// // getchar();
	// for(int i=0;i<20;i++){
	// 	cout<<p1_rec->x<<endl;
	// }


	// return counterColour;
	return counter;
}



Mat Fourier_Descriptors::Cropping_ROI(Mat imput_image,Point center_of_object, int kernel_size){
	Point temp_p;
	temp_p.x=center_of_object.x-(kernel_size/2);
	temp_p.y=center_of_object.y-(kernel_size/2);
	

	if(temp_p.x<0){
		temp_p.x=0;
	}

	if(temp_p.y<0){
		temp_p.y=0;	
	}

	if(temp_p.x+kernel_size>imput_image.cols){
		temp_p.x=imput_image.cols-kernel_size;
	}

	if(temp_p.y+kernel_size>imput_image.rows){
		temp_p.y=imput_image.rows-kernel_size;	
	}
	// cout<<"temp_p.x:"<<temp_p.x<<endl;
	// cout<<"temp_p.y:"<<temp_p.y<<endl;


	Mat ROI=imput_image(Rect(temp_p.x,temp_p.y,kernel_size,kernel_size)).clone();

	// rectangle(imput_image,Rect(temp_p.x,temp_p.y,kernel_size,kernel_size),Scalar(0,255,0),2);
	// Mat ROI=imput_image(Rect(abs(temp_p.x),abs(temp_p.y),abs(temp_p.y-center_of_object.y),abs(temp_p.y-center_of_object.y))).clone();
	return ROI;
}

int Fourier_Descriptors::Load_Imgs_Video(string p_target_file_name, string p_prev_file_name, string p_next_file_name){
	
	// prev_file_name=Extract_File_name_From_Full_Path(p_prev_file_name);
	target_file_name=Extract_File_name_From_Full_Path(p_target_file_name);
	// next_file_name=Extract_File_name_From_Full_Path(p_next_file_name);

	target_color_img=imread(p_target_file_name,1);
	target_gray_img=imread(p_target_file_name,0);
	prev_gray_img=imread(p_prev_file_name,0);
	next_gray_img=imread(p_next_file_name,0);

	bool is_next_success, is_prev_success;
	if(target_color_img.empty()==true){
		cout<<"fail to load target image"<<endl;
		type=-1;
	}else{
		type=1;
	}

	
	// type=Check_Numb_In_A_Row(p_prev_file_name, p_target_file_name, p_next_file_name);

	// cout<<"type:"<<type<<endl;
	return type;
	// if(target_color_img.empty()==true){
	// 	cout<<"fail to load target image"<<endl;
	// 	return -1;
	// }

	// if(prev_gray_img.empty()==true){
	// 	cout<<"fail to load target image"<<endl;
	// 	return -1;
	// }

	// if(target_color_img.empty()==true){
	// 	cout<<"fail to load target image"<<endl;
	// 	return -1;
	// }


	// resize(target_color_img, target_color_img, cv::Size(640,640));//resized
}

int Fourier_Descriptors::Load_Imgs(string p_target_file_name, string p_prev_file_name, string p_next_file_name){
	
	// prev_file_name=Extract_File_name_From_Full_Path(p_prev_file_name);
	target_file_name=Extract_File_name_From_Full_Path(p_target_file_name);
	// next_file_name=Extract_File_name_From_Full_Path(p_next_file_name);

	target_color_img=imread(p_target_file_name,1);
	target_gray_img=imread(p_target_file_name,0);
	prev_gray_img=imread(p_prev_file_name,0);
	next_gray_img=imread(p_next_file_name,0);

	bool is_next_success, is_prev_success;
	if(target_color_img.empty()==true){
		cout<<"fail to load target image"<<endl;
		type=-1;
	}else{
		cout<<"p2"<<endl;
		if(prev_gray_img.empty()==true){
			is_prev_success=false;
		}else{
			is_prev_success=Check_Numb_In_A_Row(p_prev_file_name, p_target_file_name);
		}

		if(next_gray_img.empty()==true){
			is_next_success=false;
		}else{
			is_next_success=Check_Numb_In_A_Row(p_next_file_name, p_target_file_name);
		}

		if(is_prev_success==false && is_next_success==false){//the last check
	        type=0;
	    }else if(is_prev_success==true && is_next_success==false){
	        type=1;
	    }else if(is_prev_success==false && is_next_success==true){
	        type=2;
	    }else if(is_prev_success==true && is_next_success==true){
	        type=3;
	    }else{
	    	type=4;
	    }
	}

	
	// type=Check_Numb_In_A_Row(p_prev_file_name, p_target_file_name, p_next_file_name);

	cout<<"type:"<<type<<endl;
	return type;
	// if(target_color_img.empty()==true){
	// 	cout<<"fail to load target image"<<endl;
	// 	return -1;
	// }

	// if(prev_gray_img.empty()==true){
	// 	cout<<"fail to load target image"<<endl;
	// 	return -1;
	// }

	// if(target_color_img.empty()==true){
	// 	cout<<"fail to load target image"<<endl;
	// 	return -1;
	// }


	// resize(target_color_img, target_color_img, cv::Size(640,640));//resized
}


bool Fourier_Descriptors::Main_The_First_Process(string p_target_file_name, string p_prev_file_name, string p_next_file_name){
	// target_file_name=p_target_file_name;
	Load_Imgs(p_target_file_name, p_prev_file_name, p_next_file_name);
	cout<<"hello"<<endl;
	// Mat temp_mat1;
	// Mat temp_mat2;
	if(type==1){
		resize(prev_gray_img, prev_gray_img, cv::Size(640,640));//resized
		resize(target_gray_img, target_gray_img, cv::Size(640,640));//resized
		resize(target_color_img, target_color_img, cv::Size(640,640));//resized
		subtracted_gray_img = prev_gray_img-target_gray_img;
	}else if(type==2){
		resize(next_gray_img, next_gray_img, cv::Size(640,640));//resized
		resize(target_gray_img, target_gray_img, cv::Size(640,640));//resized
		resize(target_color_img, target_color_img, cv::Size(640,640));//resized
		subtracted_gray_img = next_gray_img-target_gray_img;
	}else if(type==3){
		resize(prev_gray_img, prev_gray_img, cv::Size(640,640));//resized
		resize(target_gray_img, target_gray_img, cv::Size(640,640));//resized
		resize(next_gray_img, next_gray_img, cv::Size(640,640));//resized
		resize(target_color_img, target_color_img, cv::Size(640,640));//resized
		Mat temp_mat1=prev_gray_img-target_gray_img;
        Mat temp_mat2=next_gray_img-target_gray_img;
        subtracted_gray_img=temp_mat1+temp_mat2;

	}else if(type==0){
		return false;
		//not critical could not find the previous and next files
	}else if(type==-1){
		return false;
		//the error could not find the original file
	}else{
		return false;
		//unknown errors
	}
	// cout<<"hello1"<<endl;
	cv::threshold(subtracted_gray_img, thresholded_img, 0, 255, THRESH_BINARY | THRESH_OTSU);
	
    // cout<<"hello2"<<endl;
    Median_Filter(thresholded_img, thresholded_img, 9);
    
	
	// if(type==3){

	// }
 //    imwrite( "thesis_pic/prev.jpg", temp_mat1);
 //    imwrite( "thesis_pic/next.jpg", temp_mat2);
	imwrite( "thesis_pic/merged.jpg", subtracted_gray_img);
    
    Point *p_start_roi_window;
	Point *p_end_roi_window;

	p_start_roi_window=new Point[200];//approx numb
    p_end_roi_window=new Point[200];//approx numb

    int count_numb=Blob(thresholded_img, thresholded_img, p_start_roi_window, p_end_roi_window);
    if(count_numb==-100){
            // cout<<"Skip: too much blob"<<endl;
            // is_initialize_success=false;
            // cout<<"error"<<endl;
            // continue;
    	return false;

    }//segmental fault
    Point p_center_of_object=Draw_Rect_Box(target_color_img, p_start_roi_window, p_end_roi_window, 200);
    int cropping_size=Determine_Frame_Size(thresholded_img, p_center_of_object,20,20);
    if(cropping_size==-1||cropping_size==-2){
		// cout<<"sizeing error"<<endl;
		// is_initialize_success=false;
		return false;
	}

	// imshow("subtracted_gray_img",subtracted_gray_img);
 //    waitKey(0);
	target_ROI=Cropping_ROI(target_color_img, p_center_of_object,cropping_size);
	
	// imshow("thresholded_img",thresholded_img);
	// imshow("target_ROI",target_ROI);
 //    waitKey(0);

	Mat out=Cropping_ROI(thresholded_img, p_center_of_object,cropping_size);
        
    Mat out1=Cropping_ROI(target_gray_img, p_center_of_object,cropping_size);
    
    Kuwahara Kuwahara;
    out1=Kuwahara.Original_Gray_Image_To_Kuwahara_Gray_Image(out1);
    Mat final_out;
    if(type==1){//1=success prev load
        Mat out2=Cropping_ROI(prev_gray_img, p_center_of_object, cropping_size);
        out2=Kuwahara.Original_Gray_Image_To_Kuwahara_Gray_Image(out2);
        final_out=out2-out1;
    }else if(type==2){//2=success next load
        Mat out2=Cropping_ROI(next_gray_img,p_center_of_object,cropping_size);
        out2=Kuwahara.Original_Gray_Image_To_Kuwahara_Gray_Image(out2);
        final_out=out2-out1;
    }else if(type==3){//3=success next and prev load
        Mat out2=Cropping_ROI(prev_gray_img,p_center_of_object,cropping_size);
        Mat out3=Cropping_ROI(next_gray_img,p_center_of_object,cropping_size);
        out2=Kuwahara.Original_Gray_Image_To_Kuwahara_Gray_Image(out2);
        out3=Kuwahara.Original_Gray_Image_To_Kuwahara_Gray_Image(out3);
        

        Mat temp_out1=out2-out1;
        Mat temp_out2=out3-out1;
        final_out=temp_out1+temp_out2;


        // final_output=mat_collection.prev_ku_img-mat_collection.target_ku_img;
    }else{
        cout<<"error P2"<<endl;
        // getchar();
        // CE_save<<error_txt;
        // CE_save<<contour_txt;
        // CE_save<<endl;
        // continue;

    }
    cv::threshold(final_out, final_out, 0, 255, THRESH_BINARY | THRESH_OTSU);
    // medianBlur(final_out, final_out, 11);
    Median_Filter(final_out, final_out, 9);

    

    std::vector<vector<Point>>contours;

    Point ROI_mid_p;
    ROI_mid_p.x=out.rows/2;
    ROI_mid_p.y=out.cols/2;
    findContours(final_out,contours,RETR_EXTERNAL,CHAIN_APPROX_NONE);
    int object_i = Find_The_Object_Contour(contours,ROI_mid_p);
    if(object_i==-1){
    	return false;
    }
    // cout<<"hello2"<<endl;
    Mat mat_contour=Mat::zeros(final_out.size(),CV_8UC3);
	Scalar color=CV_RGB(255,0,0);

	// ROI_contour=target_ROI.clone();
	
	Point p_center_of_object2;
	p_center_of_object2.x=p_center_of_object.x+5;//because of Median Filer value 9
	p_center_of_object2.y=p_center_of_object.y+5;//because of Median Filer value 9
	Mat temp_ROI=Cropping_ROI(target_color_img, p_center_of_object2,cropping_size);
	ROI_contour=temp_ROI;

    // drawContours(ROI_contour, contours, object_i, color,3,8);
    drawContours(temp_ROI, contours, object_i, color,3,8);
    

    float target_height=final_out.cols;
	float target_width=final_out.rows;
	// cout<<"target_ori_img.rows: "<<target_ori_img.rows<<endl;
	// cout<<"target_ori_img.cols: "<<target_ori_img.cols<<endl;
	YL.yolo_height=target_height/(float)target_color_img.rows;
	YL.yolo_width=target_width/(float)target_color_img.cols;
	YL.yolo_x=p_center_of_object.x/(float)target_color_img.cols;
	YL.yolo_y=p_center_of_object.y/(float)target_color_img.rows;

	
    
    // ofstream CE_save("the_first_cycle_contours/CEs_for_test.txt");
    // vector<vector<float>>CE;
    vector<float>temp_CE;
    EllipticFourierDescriptors(contours[object_i],temp_CE);
    // cout<<"hello:"<<endl;
    contour_txt="";
    Mat el_ptr(1, n_CE, CV_32F);
    for(int i=1;i<n_CE;i++){
        contour_txt=contour_txt+to_string(temp_CE[i]);//This is for file
        el_ptr.at<float>(i-1)=temp_CE[i];//This is for contour data
        
        // strcat(contour_txt ,st );
        if(i==n_CE-1){
            contour_txt=contour_txt;
            // cout<<"enter"<<endl;

            // strcat(contour_txt, "\n");
        }else{
            contour_txt=contour_txt+",";
            // strcat(contour_txt, ",");
        }
        
    }
    cout<<"contour_txt: "<<contour_txt<<endl;
    // _data->push_back(el_ptr);

    // imshow("mat_contour",mat_contour);
    // waitKey(0);
    return true;
}
bool Fourier_Descriptors::Main_The_Second_Cycle_Process(string p_target_file_name, string p_prev_file_name, string p_next_file_name, YOLO_TXT_INFO yolo_info){
	Load_Imgs(p_target_file_name, p_prev_file_name, p_next_file_name);
	// Point ratio1;
	// Point ratio2;
	// cout<<"p1"<<endl;

	// cout<<"yolo_info.p1.x: "<<yolo_info.p1.x<<endl; 
	float p1_x=(float)yolo_info.p1.x/(float)target_color_img.cols;
	float p2_x=(float)yolo_info.p2.x/(float)target_color_img.cols;
	float p1_y=(float)yolo_info.p1.y/(float)target_color_img.rows;
	float p2_y=(float)yolo_info.p2.y/(float)target_color_img.rows;

// cout<<"p2"<<endl;
	if(type==1){
		resize(prev_gray_img, prev_gray_img, cv::Size(640,640));//resized
		resize(target_gray_img, target_gray_img, cv::Size(640,640));//resized
		resize(target_color_img, target_color_img, cv::Size(640,640));//resized
		
	}else if(type==2){
		resize(next_gray_img, next_gray_img, cv::Size(640,640));//resized
		resize(target_gray_img, target_gray_img, cv::Size(640,640));//resized
		resize(target_color_img, target_color_img, cv::Size(640,640));//resized
		
	}else if(type==3){
		resize(prev_gray_img, prev_gray_img, cv::Size(640,640));//resized
		resize(target_gray_img, target_gray_img, cv::Size(640,640));//resized
		resize(next_gray_img, next_gray_img, cv::Size(640,640));//resized
		resize(target_color_img, target_color_img, cv::Size(640,640));//resized
		
	}else if(type==0){
		return false;
		//not critical could not find the previous and next files
	}else if(type==-1){
		return false;
		//the error could not find the original file
	}else{
		return false;
		//unknown errors
	}
	Point p1,p2;

	// cout<<"p1_x:"<<p1_x<<endl;
	// cout<<"p1_y:"<<p1_y<<endl;
	// cout<<"p2_x:"<<p2_x<<endl;
	// cout<<"p2_y:"<<p2_y<<endl;

	p1.x=p1_x*(float)target_color_img.cols;
	p2.x=p2_x*(float)target_color_img.cols;
	p1.y=p1_y*(float)target_color_img.rows;
	p2.y=p2_y*(float)target_color_img.rows;


	Point center_of_object;
	center_of_object.x=(p1.x+p2.x)/2;
	center_of_object.y=(p1.y+p2.y)/2;
	// cout<<"p1.x: "<<p1.x<<endl;
	// cout<<"p1.y: "<<p1.y<<endl;
	// cout<<"p2.x: "<<p2.x<<endl;
	// cout<<"p2.y: "<<p2.y<<endl;
// cout<<"p3"<<endl;
	Rect window_frame(p1.x, p1.y, 
		        p2.x-p1.x, p2.y-p1.y);
	// cout<<"type: "<<type<<endl;

	Mat out=target_color_img(window_frame).clone();
        
    Mat out1=target_gray_img(window_frame).clone();
    target_ROI=target_color_img(window_frame).clone();
//      imshow("target_ROI",target_ROI);
// 	waitKey(0);
// cout<<"p4"<<endl;
    Kuwahara Kuwahara;
    out1=Kuwahara.Original_Gray_Image_To_Kuwahara_Gray_Image(out1);
    Mat final_out;
    if(type==1){//1=success prev load
        Mat out2=prev_gray_img(window_frame).clone();
        out2=Kuwahara.Original_Gray_Image_To_Kuwahara_Gray_Image(out2);
        final_out=out2-out1;
    }else if(type==2){//2=success next load
    	Mat out2=next_gray_img(window_frame).clone();
        // Mat out2=Cropping_ROI(next_gray_img,p_center_of_object,cropping_size);
        out2=Kuwahara.Original_Gray_Image_To_Kuwahara_Gray_Image(out2);
        final_out=out2-out1;
    }else if(type==3){//3=success next and prev load
        // Mat out2=Cropping_ROI(prev_gray_img,p_center_of_object,cropping_size);
        // Mat out3=Cropping_ROI(next_gray_img,p_center_of_object,cropping_size);
        Mat out2=prev_gray_img(window_frame).clone();
        Mat out3=next_gray_img(window_frame).clone();
        out2=Kuwahara.Original_Gray_Image_To_Kuwahara_Gray_Image(out2);
        out3=Kuwahara.Original_Gray_Image_To_Kuwahara_Gray_Image(out3);
        Mat temp_out1=out2-out1;
        Mat temp_out2=out3-out1;
        final_out=temp_out1+temp_out2;
        // final_output=mat_collection.prev_ku_img-mat_collection.target_ku_img;
    }else{
        cout<<"error P2"<<endl;
        // getchar();
        // CE_save<<error_txt;
        // CE_save<<contour_txt;
        // CE_save<<endl;
        // continue;

    }
    cv::threshold(final_out, final_out, 0, 255, THRESH_BINARY | THRESH_OTSU);
    // medianBlur(final_out, final_out, 11);
    Median_Filter(final_out, final_out, 3);


	
    std::vector<vector<Point>>contours;

    Point ROI_mid_p;
    ROI_mid_p.x=out.rows/2;
    ROI_mid_p.y=out.cols/2;
    findContours(final_out,contours,RETR_EXTERNAL,CHAIN_APPROX_NONE);
    int object_i = Find_The_Object_Contour(contours,ROI_mid_p);
    if(object_i==-1){
    	return false;
    }
    // cout<<"hello2"<<endl;
    Mat mat_contour=Mat::zeros(final_out.size(),CV_8UC3);
	Scalar color=CV_RGB(255,0,0);

	Mat temp_ROI=target_color_img(window_frame).clone();
	drawContours(temp_ROI, contours, object_i, color,3,8);
	ROI_contour=temp_ROI;
	vector<float>temp_CE;
    EllipticFourierDescriptors(contours[object_i],temp_CE);
    // cout<<"hello:"<<endl;
    contour_txt="";
    Mat el_ptr(1, n_CE, CV_32F);
    for(int i=1;i<n_CE;i++){
        contour_txt=contour_txt+to_string(temp_CE[i]);//This is for file
        el_ptr.at<float>(i-1)=temp_CE[i];//This is for contour data
        
        // strcat(contour_txt ,st );
        if(i==n_CE-1){
            contour_txt=contour_txt;
            // cout<<"enter"<<endl;

            // strcat(contour_txt, "\n");
        }else{
            contour_txt=contour_txt+",";
            // strcat(contour_txt, ",");
        }
        
    }

    

    float target_height=p2.y-p1.y;
	float target_width=p2.x-p1.x;

	YL.yolo_height=target_height/(float)target_color_img.rows;
	YL.yolo_width=target_width/(float)target_color_img.cols;
	YL.yolo_x=center_of_object.x/(float)target_color_img.cols;
	YL.yolo_y=center_of_object.y/(float)target_color_img.rows;
	return true;
}

bool Fourier_Descriptors::Main_The_Video_Cycle_Process(string p_target_file_name, string p_prev_file_name, string p_next_file_name, YOLO_TXT_INFO yolo_info){
	Load_Imgs_Video(p_target_file_name, p_prev_file_name, p_next_file_name);
	// Point ratio1;
	// Point ratio2;
	// cout<<"p1"<<endl;

	// cout<<"yolo_info.p1.x: "<<yolo_info.p1.x<<endl; 
	float p1_x=(float)yolo_info.p1.x/(float)target_color_img.cols;
	float p2_x=(float)yolo_info.p2.x/(float)target_color_img.cols;
	float p1_y=(float)yolo_info.p1.y/(float)target_color_img.rows;
	float p2_y=(float)yolo_info.p2.y/(float)target_color_img.rows;

// cout<<"p2"<<endl;
	if(type==1){
		resize(prev_gray_img, prev_gray_img, cv::Size(640,640));//resized
		resize(target_gray_img, target_gray_img, cv::Size(640,640));//resized
		resize(target_color_img, target_color_img, cv::Size(640,640));//resized
		
	}else if(type==2){
		resize(next_gray_img, next_gray_img, cv::Size(640,640));//resized
		resize(target_gray_img, target_gray_img, cv::Size(640,640));//resized
		resize(target_color_img, target_color_img, cv::Size(640,640));//resized
		
	}else if(type==3){
		resize(prev_gray_img, prev_gray_img, cv::Size(640,640));//resized
		resize(target_gray_img, target_gray_img, cv::Size(640,640));//resized
		resize(next_gray_img, next_gray_img, cv::Size(640,640));//resized
		resize(target_color_img, target_color_img, cv::Size(640,640));//resized
		
	}else if(type==0){
		return false;
		//not critical could not find the previous and next files
	}else if(type==-1){
		return false;
		//the error could not find the original file
	}else{
		return false;
		//unknown errors
	}
	Point p1,p2;

	// cout<<"p1_x:"<<p1_x<<endl;
	// cout<<"p1_y:"<<p1_y<<endl;
	// cout<<"p2_x:"<<p2_x<<endl;
	// cout<<"p2_y:"<<p2_y<<endl;

	p1.x=p1_x*(float)target_color_img.cols;
	p2.x=p2_x*(float)target_color_img.cols;
	p1.y=p1_y*(float)target_color_img.rows;
	p2.y=p2_y*(float)target_color_img.rows;


	Point center_of_object;
	center_of_object.x=(p1.x+p2.x)/2;
	center_of_object.y=(p1.y+p2.y)/2;
	// cout<<"p1.x: "<<p1.x<<endl;
	// cout<<"p1.y: "<<p1.y<<endl;
	// cout<<"p2.x: "<<p2.x<<endl;
	// cout<<"p2.y: "<<p2.y<<endl;
// cout<<"p3"<<endl;
	Rect window_frame(p1.x, p1.y, 
		        p2.x-p1.x, p2.y-p1.y);
	// cout<<"type: "<<type<<endl;

	Mat out=target_color_img(window_frame).clone();
        
    Mat out1=target_gray_img(window_frame).clone();
    target_ROI=target_color_img(window_frame).clone();
//      imshow("target_ROI",target_ROI);
// 	waitKey(0);
// cout<<"p4"<<endl;
    Kuwahara Kuwahara;
    out1=Kuwahara.Original_Gray_Image_To_Kuwahara_Gray_Image(out1);
    Mat final_out;
    if(type==1){//1=success prev load
        Mat out2=prev_gray_img(window_frame).clone();
        out2=Kuwahara.Original_Gray_Image_To_Kuwahara_Gray_Image(out2);
        final_out=out2-out1;
    }else if(type==2){//2=success next load
    	Mat out2=next_gray_img(window_frame).clone();
        // Mat out2=Cropping_ROI(next_gray_img,p_center_of_object,cropping_size);
        out2=Kuwahara.Original_Gray_Image_To_Kuwahara_Gray_Image(out2);
        final_out=out2-out1;
    }else if(type==3){//3=success next and prev load
        // Mat out2=Cropping_ROI(prev_gray_img,p_center_of_object,cropping_size);
        // Mat out3=Cropping_ROI(next_gray_img,p_center_of_object,cropping_size);
        Mat out2=prev_gray_img(window_frame).clone();
        Mat out3=next_gray_img(window_frame).clone();
        out2=Kuwahara.Original_Gray_Image_To_Kuwahara_Gray_Image(out2);
        out3=Kuwahara.Original_Gray_Image_To_Kuwahara_Gray_Image(out3);
        Mat temp_out1=out2-out1;
        Mat temp_out2=out3-out1;
        final_out=temp_out1+temp_out2;
        // final_output=mat_collection.prev_ku_img-mat_collection.target_ku_img;
    }else{
        cout<<"error P2"<<endl;
        // getchar();
        // CE_save<<error_txt;
        // CE_save<<contour_txt;
        // CE_save<<endl;
        // continue;

    }
    cv::threshold(final_out, final_out, 0, 255, THRESH_BINARY | THRESH_OTSU);
    // medianBlur(final_out, final_out, 11);
    Median_Filter(final_out, final_out, 3);


	
    std::vector<vector<Point>>contours;

    Point ROI_mid_p;
    ROI_mid_p.x=out.rows/2;
    ROI_mid_p.y=out.cols/2;
    findContours(final_out,contours,RETR_EXTERNAL,CHAIN_APPROX_NONE);
    int object_i = Find_The_Object_Contour(contours,ROI_mid_p);
    if(object_i==-1){
    	return false;
    }
    // cout<<"hello2"<<endl;
    Mat mat_contour=Mat::zeros(final_out.size(),CV_8UC3);
	Scalar color=CV_RGB(255,0,0);

	Mat temp_ROI=target_color_img(window_frame).clone();
	drawContours(temp_ROI, contours, object_i, color,3,8);
	ROI_contour=temp_ROI;
	vector<float>temp_CE;
    EllipticFourierDescriptors(contours[object_i],temp_CE);
    // cout<<"hello:"<<endl;
    contour_txt="";
    Mat el_ptr(1, n_CE, CV_32F);
    for(int i=1;i<n_CE;i++){
        contour_txt=contour_txt+to_string(temp_CE[i]);//This is for file
        el_ptr.at<float>(i-1)=temp_CE[i];//This is for contour data
        
        // strcat(contour_txt ,st );
        if(i==n_CE-1){
            contour_txt=contour_txt;
            // cout<<"enter"<<endl;

            // strcat(contour_txt, "\n");
        }else{
            contour_txt=contour_txt+",";
            // strcat(contour_txt, ",");
        }
        
    }

    

    float target_height=p2.y-p1.y;
	float target_width=p2.x-p1.x;

	YL.yolo_height=target_height/(float)target_color_img.rows;
	YL.yolo_width=target_width/(float)target_color_img.cols;
	YL.yolo_x=center_of_object.x/(float)target_color_img.cols;
	YL.yolo_y=center_of_object.y/(float)target_color_img.rows;
	return true;
}


void Main_Contour_Yolo_Creater(const string class_number,const string path_dataset,const string idx_value){
	cout<<"class_number: "<<class_number<<endl;
	// cout<<"path_dataset: "<<p_function1.path_dataset<<endl;
	// cout<<"idx_value: "<<p_function1.idx_value<<endl;
	string filename_parent=Extract_File_name_From_Full_Path(path_dataset);
	filename_parent=filename_parent+"/";
	cout<<"filename_parent: "<<filename_parent<<endl;
	glob_t glob_result=Read_Dataset_Dir(path_dataset);
	// cout<<"glob_result.gl_pathc: "<<glob_result.gl_pathc<<endl;
	

  	string folder_name_root="output_the_first_contour_yolo/";
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

	cout<<"hello world"<<endl;
	unsigned int i;//in order to save index later
	cout<<"glob_result.gl_pathc: "<<glob_result.gl_pathc<<endl;
	for(i=stoi(idx_value); i<glob_result.gl_pathc; i++){

		string target_file_name, prev_file_name, next_file_name;
		target_file_name=glob_result.gl_pathv[i];
		if(i==0){
			
			prev_file_name="";
		}else{
			prev_file_name=glob_result.gl_pathv[i-1];
		}

		if(i+1>=glob_result.gl_pathc){
			next_file_name="";
		}else{
			next_file_name=glob_result.gl_pathv[i+1];
		}

		// cout<<"prev_file_name: "<<prev_file_name<<endl;
		// cout<<"target_file_name: "<<target_file_name<<endl;
		// cout<<"next_file_name: "<<next_file_name<<endl;

		Fourier_Descriptors FD;

		bool is_success=FD.Main_The_First_Process(target_file_name, prev_file_name, next_file_name);
		// Directory_Creater

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
		


		// write_file.Main_Process(
  // 			ku_ROI.get_target_ROI_img(),
  // 			ku_ROI.get_ROI_and_drawing(),
  // 			ku_ROI.get_target_original_img(),
  // 			file_name,
  // 			class_number,
  // 			ku_ROI.get_yolo_x(),
  // 			ku_ROI.get_yolo_y(),
  // 			ku_ROI.get_yolo_width(),
  // 			ku_ROI.get_yolo_height(),
  // 			ku_ROI.get_contour_txt()
  // 		);
		// FD.Load_Imgs(target_file_name,prev_file_name, next_file_name);
		// FD.t();
		
		// Mat temp_img=FD.get_target_color_img();
		// imshow("ori",temp_img);
		// waitKey(0);
		// cout<<"i: "<<i<<endl;
		// getchar();


		// Mat temp_target_img=imread(temp_file_name,0);
  //   	Mat temp_color_target_img=imread(full_target_path,1);


		// Test_Matrices mat_collection=part1(file_root, imgname[i],data_root1, data_root2);
	}
	i++;//next one
	bool is_success_to_save_index=Index_Saver(folder_name_root+filename_parent+"/index.txt",i);	

}
Mat Fourier_Descriptors::Main_Detection_V1(Mat back_ground_img, Mat current_img){
	Mat back_ground_gray_img;
	Mat current_gray_img;
	cvtColor(back_ground_img, back_ground_gray_img, COLOR_BGR2GRAY);//copy camera color image to gray scale
   	cvtColor(current_img, current_gray_img, COLOR_BGR2GRAY);//copy camera color image to gray scale
	

	Kuwahara Kuwahara;
    back_ground_gray_img=Kuwahara.Original_Gray_Image_To_Kuwahara_Gray_Image(back_ground_gray_img);
    current_gray_img=Kuwahara.Original_Gray_Image_To_Kuwahara_Gray_Image(current_gray_img);
    Mat final_out;
    final_out=back_ground_gray_img-current_gray_img;
    cv::threshold(final_out, final_out, 0, 255, THRESH_BINARY | THRESH_OTSU);
 //    // medianBlur(final_out, final_out, 11);
    Median_Filter(final_out, final_out, 3);
    std::vector<vector<Point>>contours;

    Point ROI_mid_p;
    ROI_mid_p.x=final_out.rows/2;
    ROI_mid_p.y=final_out.cols/2;
    findContours(final_out,contours,RETR_EXTERNAL,CHAIN_APPROX_NONE);
    int object_i = Find_The_Object_Contour(contours,ROI_mid_p);
    if(object_i==-1){
    	return final_out;
    }
    Mat mat_contour=Mat::zeros(final_out.size(),CV_8UC3);
	Scalar color=CV_RGB(255,0,0);

	// // Mat temp_ROI=target_color_img(window_frame).clone();
	// Mat mat_contour=Mat::zeros(final_out.size(),CV_8UC3);
	drawContours(mat_contour, contours, object_i, color,3,8);
	return mat_contour;
}
Mat Fourier_Descriptors::Main_Detection_V2(Mat back_ground_img, Mat current_img){
	Mat back_ground_gray_img;
	Mat current_gray_img;
	cvtColor(back_ground_img, back_ground_gray_img, COLOR_BGR2GRAY);//copy camera color image to gray scale
   	cvtColor(current_img, current_gray_img, COLOR_BGR2GRAY);//copy camera color image to gray scale
	Mat subtracted_gray_img = back_ground_gray_img-current_gray_img;
	Mat thresholded_img=subtracted_gray_img;
	// cv::threshold(subtracted_gray_img, thresholded_img, 0, 70, THRESH_BINARY );
	int t_p=thresholding_image(thresholded_img, 70,true,0);
	
    // cout<<"hello2"<<endl;
    Median_Filter(thresholded_img, thresholded_img, 9);
    Point *p_start_roi_window;
	Point *p_end_roi_window;

	p_start_roi_window=new Point[200];//approx numb
    p_end_roi_window=new Point[200];//approx numb

    int count_numb=Blob(thresholded_img, thresholded_img, p_start_roi_window, p_end_roi_window);
    if(count_numb==-100){
            cout<<"Skip: too much blob"<<endl;
            // is_initialize_success=false;
            // cout<<"error"<<endl;
            // continue;
    	return current_img;

    }//segmental fault
    Point p_center_of_object=Draw_Rect_Box2(current_img, p_start_roi_window, p_end_roi_window, 200);
 	
 //    int cropping_size=Determine_Frame_Size(thresholded_img, p_center_of_object,20,20);
 //    if(cropping_size==-1||cropping_size==-2){
	// 	cout<<"sizeing error"<<endl;
	// 	// is_initialize_success=false;
	// 	return current_img;
	// }
	std::vector<vector<Point>>contours;

	// Point ROI_mid_p;
 //    ROI_mid_p.x=final_out.rows/2;
 //    ROI_mid_p.y=final_out.cols/2;
    Scalar color=CV_RGB(255,0,0);
    findContours(thresholded_img,contours,RETR_EXTERNAL,CHAIN_APPROX_NONE);

    int object_i = Find_The_Object_Contour(contours,p_center_of_object);
    
    if(object_i==-1){
    	return current_img;
    }

	
	vector<float>temp_CE;
    EllipticFourierDescriptors(contours[object_i],temp_CE);
    // std::vector<vector<Point>>contours;
    // cout<<"hello:"<<endl;
    Mat temp_data(1, n_CE, CV_32F);
    for(int i=1;i<n_CE;i++){
        temp_data.at<float>(i-1)=temp_CE[i];//This is for contour data
    }
    final_CE_data=temp_data;
    drawContours(current_img, contours, object_i, color,3,8);
    // cout<<"process done well"<<endl;

	// imshow("subtracted_gray_img",subtracted_gray_img);
 //    waitKey(0);
	// target_ROI=Cropping_ROI(target_color_img, p_center_of_object,cropping_size);


	// Kuwahara Kuwahara;
 //    back_ground_gray_img=Kuwahara.Original_Gray_Image_To_Kuwahara_Gray_Image(back_ground_gray_img);
 //    current_gray_img=Kuwahara.Original_Gray_Image_To_Kuwahara_Gray_Image(current_gray_img);
 //    Mat final_out;
 //    final_out=back_ground_gray_img-current_gray_img;
 //    cv::threshold(final_out, final_out, 0, 255, THRESH_BINARY | THRESH_OTSU);
 // //    // medianBlur(final_out, final_out, 11);
 //    Median_Filter(final_out, final_out, 3);
    

 //    Point ROI_mid_p;
 //    ROI_mid_p.x=final_out.rows/2;
 //    ROI_mid_p.y=final_out.cols/2;
 //    findContours(final_out,contours,RETR_EXTERNAL,CHAIN_APPROX_NONE);
 //    int object_i = Find_The_Object_Contour(contours,ROI_mid_p);
 //    if(object_i==-1){
 //    	return final_out;
 //    }
 //    Mat mat_contour=Mat::zeros(final_out.size(),CV_8UC3);
	// Scalar color=CV_RGB(255,0,0);

	// // // Mat temp_ROI=target_color_img(window_frame).clone();
	// // Mat mat_contour=Mat::zeros(final_out.size(),CV_8UC3);
	// drawContours(mat_contour, contours, object_i, color,3,8);
	return current_img;
}


// bool Fourier_Descriptors::Main_Detection(string p_target_file_name, string p_prev_file_name, string p_next_file_name, YOLO_TXT_INFO yolo_info){
// 	Load_Imgs(p_target_file_name, p_prev_file_name, p_next_file_name);
// 	// Point ratio1;
// 	// Point ratio2;
// 	// cout<<"p1"<<endl;

// 	// cout<<"yolo_info.p1.x: "<<yolo_info.p1.x<<endl; 
// 	float p1_x=(float)yolo_info.p1.x/(float)target_color_img.cols;
// 	float p2_x=(float)yolo_info.p2.x/(float)target_color_img.cols;
// 	float p1_y=(float)yolo_info.p1.y/(float)target_color_img.rows;
// 	float p2_y=(float)yolo_info.p2.y/(float)target_color_img.rows;

// // cout<<"p2"<<endl;
// 	if(type==1){
// 		resize(prev_gray_img, prev_gray_img, cv::Size(640,640));//resized
// 		resize(target_gray_img, target_gray_img, cv::Size(640,640));//resized
// 		resize(target_color_img, target_color_img, cv::Size(640,640));//resized
		
// 	}else if(type==2){
// 		resize(next_gray_img, next_gray_img, cv::Size(640,640));//resized
// 		resize(target_gray_img, target_gray_img, cv::Size(640,640));//resized
// 		resize(target_color_img, target_color_img, cv::Size(640,640));//resized
		
// 	}else if(type==3){
// 		resize(prev_gray_img, prev_gray_img, cv::Size(640,640));//resized
// 		resize(target_gray_img, target_gray_img, cv::Size(640,640));//resized
// 		resize(next_gray_img, next_gray_img, cv::Size(640,640));//resized
// 		resize(target_color_img, target_color_img, cv::Size(640,640));//resized
		
// 	}else if(type==0){
// 		return false;
// 		//not critical could not find the previous and next files
// 	}else if(type==-1){
// 		return false;
// 		//the error could not find the original file
// 	}else{
// 		return false;
// 		//unknown errors
// 	}
// 	Point p1,p2;

// 	// cout<<"p1_x:"<<p1_x<<endl;
// 	// cout<<"p1_y:"<<p1_y<<endl;
// 	// cout<<"p2_x:"<<p2_x<<endl;
// 	// cout<<"p2_y:"<<p2_y<<endl;

// 	p1.x=p1_x*(float)target_color_img.cols;
// 	p2.x=p2_x*(float)target_color_img.cols;
// 	p1.y=p1_y*(float)target_color_img.rows;
// 	p2.y=p2_y*(float)target_color_img.rows;


// 	Point center_of_object;
// 	center_of_object.x=(p1.x+p2.x)/2;
// 	center_of_object.y=(p1.y+p2.y)/2;
// 	// cout<<"p1.x: "<<p1.x<<endl;
// 	// cout<<"p1.y: "<<p1.y<<endl;
// 	// cout<<"p2.x: "<<p2.x<<endl;
// 	// cout<<"p2.y: "<<p2.y<<endl;
// // cout<<"p3"<<endl;
// 	Rect window_frame(p1.x, p1.y, 
// 		        p2.x-p1.x, p2.y-p1.y);
// 	// cout<<"type: "<<type<<endl;

// 	Mat out=target_color_img(window_frame).clone();
        
//     Mat out1=target_gray_img(window_frame).clone();
//     target_ROI=target_color_img(window_frame).clone();
// //      imshow("target_ROI",target_ROI);
// // 	waitKey(0);
// // cout<<"p4"<<endl;
//     Kuwahara Kuwahara;
//     out1=Kuwahara.Original_Gray_Image_To_Kuwahara_Gray_Image(out1);
//     Mat final_out;
//     if(type==1){//1=success prev load
//         Mat out2=prev_gray_img(window_frame).clone();
//         out2=Kuwahara.Original_Gray_Image_To_Kuwahara_Gray_Image(out2);
//         final_out=out2-out1;
//     }else if(type==2){//2=success next load
//     	Mat out2=next_gray_img(window_frame).clone();
//         // Mat out2=Cropping_ROI(next_gray_img,p_center_of_object,cropping_size);
//         out2=Kuwahara.Original_Gray_Image_To_Kuwahara_Gray_Image(out2);
//         final_out=out2-out1;
//     }else if(type==3){//3=success next and prev load
//         // Mat out2=Cropping_ROI(prev_gray_img,p_center_of_object,cropping_size);
//         // Mat out3=Cropping_ROI(next_gray_img,p_center_of_object,cropping_size);
//         Mat out2=prev_gray_img(window_frame).clone();
//         Mat out3=next_gray_img(window_frame).clone();
//         out2=Kuwahara.Original_Gray_Image_To_Kuwahara_Gray_Image(out2);
//         out3=Kuwahara.Original_Gray_Image_To_Kuwahara_Gray_Image(out3);
//         Mat temp_out1=out2-out1;
//         Mat temp_out2=out3-out1;
//         final_out=temp_out1+temp_out2;
//         // final_output=mat_collection.prev_ku_img-mat_collection.target_ku_img;
//     }else{
//         cout<<"error P2"<<endl;
//         // getchar();
//         // CE_save<<error_txt;
//         // CE_save<<contour_txt;
//         // CE_save<<endl;
//         // continue;

//     }
//     cv::threshold(final_out, final_out, 0, 255, THRESH_BINARY | THRESH_OTSU);
//     // medianBlur(final_out, final_out, 11);
//     Median_Filter(final_out, final_out, 3);


	
//     std::vector<vector<Point>>contours;

//     Point ROI_mid_p;
//     ROI_mid_p.x=out.rows/2;
//     ROI_mid_p.y=out.cols/2;
//     findContours(final_out,contours,RETR_EXTERNAL,CHAIN_APPROX_NONE);
//     int object_i = Find_The_Object_Contour(contours,ROI_mid_p);
//     if(object_i==-1){
//     	return false;
//     }
//     // cout<<"hello2"<<endl;
//     Mat mat_contour=Mat::zeros(final_out.size(),CV_8UC3);
// 	Scalar color=CV_RGB(255,0,0);

// 	Mat temp_ROI=target_color_img(window_frame).clone();
// 	drawContours(temp_ROI, contours, object_i, color,3,8);
// 	ROI_contour=temp_ROI;
// 	vector<float>temp_CE;
//     EllipticFourierDescriptors(contours[object_i],temp_CE);
//     // cout<<"hello:"<<endl;
//     contour_txt="";
//     Mat el_ptr(1, n_CE, CV_32F);
//     for(int i=1;i<n_CE;i++){
//         contour_txt=contour_txt+to_string(temp_CE[i]);//This is for file
//         el_ptr.at<float>(i-1)=temp_CE[i];//This is for contour data
        
//         // strcat(contour_txt ,st );
//         if(i==n_CE-1){
//             contour_txt=contour_txt;
//             // cout<<"enter"<<endl;

//             // strcat(contour_txt, "\n");
//         }else{
//             contour_txt=contour_txt+",";
//             // strcat(contour_txt, ",");
//         }
        
//     }

    

//     float target_height=p2.y-p1.y;
// 	float target_width=p2.x-p1.x;

// 	YL.yolo_height=target_height/(float)target_color_img.rows;
// 	YL.yolo_width=target_width/(float)target_color_img.cols;
// 	YL.yolo_x=center_of_object.x/(float)target_color_img.cols;
// 	YL.yolo_y=center_of_object.y/(float)target_color_img.rows;
// 	return true;
// }