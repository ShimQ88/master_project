#include "kuwahara.h"

void Kuwahara::Integral_Gray_Initialize(Mat image1, double **integral_image,double **squared_integral_image){
	//This is the function for creating summed-area table in gray image.
	for(int i=0;i<image1.cols+1;i++){
		for(int j=0;j<image1.rows+1;j++){
			if((i==0)||(j==0)){//The value of the first row and column is zero. 
				integral_image[i][j]=0;
				squared_integral_image[i][j]=0;
			}else{
				integral_image[i][j]=integral_image[i-1][j]+integral_image[i][j-1]-integral_image[i-1][j-1]+(int)Mpixel(image1,i-1,j-1);
				squared_integral_image[i][j]=squared_integral_image[i-1][j]+squared_integral_image[i][j-1]-squared_integral_image[i-1][j-1]
				+(int)Mpixel(image1,i-1,j-1)*(int)Mpixel(image1,i-1,j-1);
			}
			
		}
	}
}
void Kuwahara::Integral_Color_Initialize(Mat3b image1, double **integral_image_B, double **integral_image_G, double **integral_image_R, double **squared_integral_image_B, double **squared_integral_image_G, double **squared_integral_image_R){
	//This is the function for creating summed-area table in color image.
	for(int i=0;i<image1.cols;i++){
		for(int j=0;j<image1.rows;j++){
			if((i==0)||(j==0)){//The value of the first row and column is zero.
				integral_image_B[i][j]=0;
				integral_image_G[i][j]=0;
				integral_image_R[i][j]=0;

				squared_integral_image_B[i][j]=0;
				squared_integral_image_G[i][j]=0;
				squared_integral_image_R[i][j]=0;
			}else{
				integral_image_B[i][j]=integral_image_B[i-1][j]+integral_image_B[i][j-1]-integral_image_B[i-1][j-1]+(int)pixelB(image1,i-1,j-1);
				integral_image_G[i][j]=integral_image_G[i-1][j]+integral_image_G[i][j-1]-integral_image_G[i-1][j-1]+(int)pixelG(image1,i-1,j-1);
				integral_image_R[i][j]=integral_image_R[i-1][j]+integral_image_R[i][j-1]-integral_image_R[i-1][j-1]+(int)pixelR(image1,i-1,j-1);

				squared_integral_image_B[i][j]=squared_integral_image_B[i-1][j]+squared_integral_image_B[i][j-1]-squared_integral_image_B[i-1][j-1]+(int)pixelB(image1,i-1,j-1)*(int)pixelB(image1,i-1,j-1);
				squared_integral_image_G[i][j]=squared_integral_image_G[i-1][j]+squared_integral_image_G[i][j-1]-squared_integral_image_G[i-1][j-1]+(int)pixelG(image1,i-1,j-1)*(int)pixelG(image1,i-1,j-1);
				squared_integral_image_R[i][j]=squared_integral_image_R[i-1][j]+squared_integral_image_R[i][j-1]-squared_integral_image_R[i-1][j-1]+(int)pixelR(image1,i-1,j-1)*(int)pixelR(image1,i-1,j-1);
			}
			
		}

	}
}
void Kuwahara::Kuwahara_Filter_Gray_With_Sum_Table(Mat source_image, Mat output_image, double** integral_image, double** squared_integral_image, int window_size){
	//This is function for applying the Kuwahara filter to output_image, it must require summed-area table.

	//Mat source_image is the source image with gray scale. 
	//Mat output_image is the final output which is applied to Kuwahara filter in gray scale image.
	//double** integral_image is 2-dimensional array which stores the summed-area table.
	//int window_size is the size of window it is big more blurly when the value is high.

	int picture_x_size=source_image.cols;//the x-axis length of source image.
	int picture_y_size=source_image.rows;//the y-axis length of source image.
	

	for(int i=0;i<=source_image.cols-(window_size);i++){
		for(int j=0;j<=source_image.rows-(window_size);j++){
			double f[4];
			double s_f[4];
			double result[4];
			int small_window_size=(window_size/2+1)*(window_size/2+1);

			int i_col[4];
			int i_row[4];

			i_col[0]=i;
			i_row[0]=j;

			i_col[1]=i+(window_size/2);
			i_row[1]=j;

			i_col[2]=i;
			i_row[2]=j+(window_size/2);

			i_col[3]=i+(window_size/2);
			i_row[3]=j+(window_size/2);

			double mean_fa[4];

			
			for(int a=0;a<4;a++){
				f[a]=integral_image[i_col[a]+(window_size/2)+1][i_row[a]+(window_size/2)+1]-integral_image[i_col[a]][i_row[a]+(window_size/2)+1]
				-integral_image[i_col[a]+(window_size/2)+1][i_row[a]]+integral_image[i_col[a]][i_row[a]];

				s_f[a]=squared_integral_image[i_col[a]+(window_size/2)+1][i_row[a]+(window_size/2)+1]-squared_integral_image[i_col[a]][i_row[a]+(window_size/2)+1]
				-squared_integral_image[i_col[a]+(window_size/2)+1][i_row[a]]+squared_integral_image[i_col[a]][i_row[a]];

				mean_fa[a]=f[a]/small_window_size;
				// cout<<"mean_fa["<<a<<"]: "<<mean_fa[a]<<endl;
			}
			for(int a=0;a<4;a++){
				result[a]=(s_f[a]-((f[a]*f[a])/small_window_size))/small_window_size;
			}


			double final=9999;
			int min_index=0;
			for(int l=0;l<4;l++){
				if(final>result[l]){
					final=result[l];
					min_index=l;
				}
			}
			Mpixel(output_image,i+window_size/2,j+window_size/2)=(int)mean_fa[min_index];
		}
	}

}

void Kuwahara::Kuwahara_Filter_Gray_Without_Sum_Table(Mat source_image, Mat output_image, int window_size){
	//This is function for applying the Kuwahara filter to output_image, it must require summed-area table.

	//Mat source_image is the source image with gray scale. 
	//Mat output_image is the final output which is applied to Kuwahara filter in gray scale image.
	//double** integral_image is 2-dimensional array which stores the summed-area table.
	//int window_size is the size of window it is big more blurly when the value is high.

	int picture_x_size=source_image.cols;//the x-axis length of source image.
	int picture_y_size=source_image.rows;//the y-axis length of source image.


	
	for(int i=0;i<=source_image.cols-(window_size);i++){
		for(int j=0;j<=source_image.rows-(window_size);j++){
			double f[4];
			double s_f[4];
			double result[4];
			int small_window_size=(window_size/2+1)*(window_size/2+1);

			int i_col[4];
			int i_row[4];

			i_col[0]=i;
			i_row[0]=j;

			i_col[1]=i+(window_size/2);
			i_row[1]=j;

			i_col[2]=i;
			i_row[2]=j+(window_size/2);

			i_col[3]=i+(window_size/2);
			i_row[3]=j+(window_size/2);

			double mean_f[4];

			// cout<<"small_window_size: "<<small_window_size<<endl;

			mean_f[0]=0;
			for(int l=i;l<=i+(window_size/2);l++){
				for(int k=j;k<=j+(window_size/2);k++){
					mean_f[0]=mean_f[0]+(float)Mpixel(source_image,l,k);
				}
			}
			mean_f[0]=mean_f[0]/small_window_size;

			// cout<<"mean_f[0]: "<<mean_f[0]<<endl;
			
			//the section of f[1]
			//  | |*|*|        
			//	| |*|*|
			//	| | | |
			mean_f[1]=0;
			for(int l=i+(window_size/2);l<i+window_size;l++){
				for(int k=j;k<=j+(window_size/2);k++){
					mean_f[1]=mean_f[1]+(float)Mpixel(source_image,l,k);
				}
			}
			mean_f[1]=mean_f[1]/small_window_size;

			// cout<<"mean_f[1]: "<<mean_f[1]<<endl;

			//the section of f[2]
			//  | | | |        
			//	|*|*| |
			//	|*|*| |

			mean_f[2]=0;
			for(int l=i;l<=i+(window_size/2);l++){
				for(int k=j+(window_size/2);k<j+window_size;k++){
					mean_f[2]=mean_f[2]+(float)Mpixel(source_image,l,k);
				}
			}
			mean_f[2]=mean_f[2]/small_window_size;
			// cout<<"mean_f[2]: "<<mean_f[2]<<endl;


			//the section of f[3]
			//  | | | |        
			//	| |*|*|
			//	| |*|*|

			mean_f[3]=0;
			for(int l=i+(window_size/2);l<i+window_size;l++){
				for(int k=j+(window_size/2);k<j+window_size;k++){
					mean_f[3]=mean_f[3]+(float)Mpixel(source_image,l,k);
				}
			}

			mean_f[3]=mean_f[3]/small_window_size;
			// cout<<"mean_f[3]: "<<mean_f[3]<<endl;
			
			

			double temp=0;
			result[0]=0;
			for(int l=i;l<=i+(window_size/2);l++){
				for(int k=j;k<=j+(window_size/2);k++){
					temp=mean_f[0]-(double)Mpixel(source_image,l,k);
					temp=temp*temp;
					result[0]=result[0]+temp;
				}
			}

			result[0]=result[0]/small_window_size;


			
			//the section of f[1]
			//  | |*|*|        
			//	| |*|*|
			//	| | | |
			result[1]=0;
			for(int l=i+(window_size/2);l<i+window_size;l++){
				for(int k=j;k<=j+(window_size/2);k++){
					temp=mean_f[1]-(double)Mpixel(source_image,l,k);
					temp=temp*temp;
					result[1]=result[1]+temp;
				}
			}
			result[1]=result[1]/small_window_size;


			//the section of f[2]
			//  | | | |        
			//	|*|*| |
			//	|*|*| |
			result[2]=0;
			for(int l=i;l<=i+(window_size/2);l++){
				for(int k=j+(window_size/2);k<j+window_size;k++){
					temp=mean_f[2]-(double)Mpixel(source_image,l,k);
					temp=temp*temp;
					result[2]=result[2]+temp;
				}
			}
			result[2]=result[2]/small_window_size;


			//the section of f[3]
			//  | | | |        
			//	| |*|*|
			//	| |*|*|
			result[3]=0;
			for(int l=i+(window_size/2);l<i+window_size;l++){
				for(int k=j+(window_size/2);k<j+window_size;k++){
					temp=mean_f[3]-(double)Mpixel(source_image,l,k);
					temp=temp*temp;
					// cout<<"temp: "<<temp<<endl;
					result[3]=result[3]+temp;
				}
			}
			// getchar();

			result[3]=result[3]/small_window_size;

			double final=9999;
			int min_index=0;
			for(int l=0;l<4;l++){
				if(final>result[l]){
					final=result[l];
					min_index=l;
				}
			}
			Mpixel(output_image,i+window_size/2,j+window_size/2)=(int)mean_f[min_index];
		}
	}

}
void Kuwahara::Kuwahara_Filter_Color_With_Sum_Table(Mat3b source_image, Mat3b output_image, double **integral_image_B, double **integral_image_G, double **integral_image_R, double **squared_integral_image_B, double **squared_integral_image_G, double **squared_integral_image_R, int window_size){
	//This is function for applying the Kuwahara filter to output_image, it must require summed-area table.

	//Mat source_image is the source image with gray scale. 
	//Mat output_image is the final output which is applied to Kuwahara filter in gray scale image.
	//double** integral_image is 2-dimensional array which stores the summed-area table.
	//int window_size is the size of window it is big more blurly when the value is high.

	int picture_x_size=source_image.cols;//the x-axis length of source image.
	int picture_y_size=source_image.rows;//the y-axis length of source image.
	

	for(int i=0;i<=source_image.cols-(window_size);i++){
		for(int j=0;j<=source_image.rows-(window_size);j++){
			double f_B[4];
			double f_G[4];
			double f_R[4];
			double s_f_B[4];
			double s_f_G[4];
			double s_f_R[4];
			double result_B[4];
			double result_G[4];
			double result_R[4];
			int small_window_size=(window_size/2+1)*(window_size/2+1);

			int i_col[4];
			int i_row[4];

			i_col[0]=i;
			i_row[0]=j;

			i_col[1]=i+(window_size/2);
			i_row[1]=j;

			i_col[2]=i;
			i_row[2]=j+(window_size/2);

			i_col[3]=i+(window_size/2);
			i_row[3]=j+(window_size/2);

			double mean_fa_B[4];
			double mean_fa_G[4];
			double mean_fa_R[4];

			for(int a=0;a<4;a++){
				f_B[a]=integral_image_B[i_col[a]+(window_size/2)+1][i_row[a]+(window_size/2)+1]-integral_image_B[i_col[a]][i_row[a]+(window_size/2)+1]
				-integral_image_B[i_col[a]+(window_size/2)+1][i_row[a]]+integral_image_B[i_col[a]][i_row[a]];

				s_f_B[a]=squared_integral_image_B[i_col[a]+(window_size/2)+1][i_row[a]+(window_size/2)+1]-squared_integral_image_B[i_col[a]][i_row[a]+(window_size/2)+1]
				-squared_integral_image_B[i_col[a]+(window_size/2)+1][i_row[a]]+squared_integral_image_B[i_col[a]][i_row[a]];

				mean_fa_B[a]=f_B[a]/small_window_size;
				// cout<<"mean_fa_B["<<a<<"]: "<<mean_fa_B[a]<<endl;
			}

			for(int a=0;a<4;a++){
				f_G[a]=integral_image_G[i_col[a]+(window_size/2)+1][i_row[a]+(window_size/2)+1]-integral_image_G[i_col[a]][i_row[a]+(window_size/2)+1]
				-integral_image_G[i_col[a]+(window_size/2)+1][i_row[a]]+integral_image_G[i_col[a]][i_row[a]];

				s_f_G[a]=squared_integral_image_G[i_col[a]+(window_size/2)+1][i_row[a]+(window_size/2)+1]-squared_integral_image_G[i_col[a]][i_row[a]+(window_size/2)+1]
				-squared_integral_image_G[i_col[a]+(window_size/2)+1][i_row[a]]+squared_integral_image_G[i_col[a]][i_row[a]];

				mean_fa_G[a]=f_G[a]/small_window_size;
				// cout<<"mean_fa_G["<<a<<"]: "<<mean_fa_G[a]<<endl;
			}
			
			for(int a=0;a<4;a++){
				f_R[a]=integral_image_R[i_col[a]+(window_size/2)+1][i_row[a]+(window_size/2)+1]-integral_image_R[i_col[a]][i_row[a]+(window_size/2)+1]
				-integral_image_R[i_col[a]+(window_size/2)+1][i_row[a]]+integral_image_R[i_col[a]][i_row[a]];

				s_f_R[a]=squared_integral_image_R[i_col[a]+(window_size/2)+1][i_row[a]+(window_size/2)+1]-squared_integral_image_R[i_col[a]][i_row[a]+(window_size/2)+1]
				-squared_integral_image_R[i_col[a]+(window_size/2)+1][i_row[a]]+squared_integral_image_R[i_col[a]][i_row[a]];

				mean_fa_R[a]=f_R[a]/small_window_size;
				// cout<<"mean_fa_R["<<a<<"]: "<<mean_fa_R[a]<<endl;
			}

			for(int a=0;a<4;a++){
				result_B[a]=(s_f_B[a]-((f_B[a]*f_B[a])/small_window_size))/small_window_size;
			}

			for(int a=0;a<4;a++){
				result_G[a]=(s_f_G[a]-((f_G[a]*f_G[a])/small_window_size))/small_window_size;
			}

			for(int a=0;a<4;a++){
				result_R[a]=(s_f_R[a]-((f_R[a]*f_R[a])/small_window_size))/small_window_size;
			}



			double final=9999;
			int min_index=0;
			for(int l=0;l<4;l++){
				if(final>result_B[l]){
					final=result_B[l];
					min_index=l;
				}
			}

			pixelB(output_image,i+window_size/2,j+window_size/2)=(int)mean_fa_B[min_index];

			final=9999;
			min_index=0;
			for(int l=0;l<4;l++){
				if(final>result_G[l]){
					final=result_G[l];
					min_index=l;
				}
			}

			pixelG(output_image,i+window_size/2,j+window_size/2)=(int)mean_fa_G[min_index];

			final=9999;
			min_index=0;

			for(int l=0;l<4;l++){
				if(final>result_R[l]){
					final=result_R[l];
					min_index=l;
				}
			}

			pixelR(output_image,i+window_size/2,j+window_size/2)=(int)mean_fa_R[min_index];
		}
	}

}




Mat Kuwahara::Original_Gray_Image_To_Kuwahara_Gray_Image(Mat input_gray){
    /*Memory Allocation*/
    // cout<<"input_gray.size(): "<<input_gray.size()<<endl;
    Mat output=Mat::zeros(input_gray.size(), IMREAD_GRAYSCALE);//initialize the value of output metrices to zero
    double** integral_image=new double*[input_gray.cols+1];
    double** squared_integral_image=new double*[input_gray.cols+1];
    for(int i = 0; i < input_gray.cols+1; ++i){
        integral_image[i] = new double[input_gray.rows+1];
        squared_integral_image[i] = new double[input_gray.rows+1];
    }

    int kuwahara_level=11;//have to be odd number
    Integral_Gray_Initialize(input_gray,integral_image,squared_integral_image);//create summed-table to integral_image array.
    Kuwahara_Filter_Gray_With_Sum_Table(input_gray,output,integral_image,squared_integral_image,kuwahara_level);//Applying kuwahara filter to output using integral_image.
    
    /*Memory deallocation*/
    for(int i = 0; i < input_gray.cols+1; ++i) {
        delete [] integral_image[i];
        delete [] squared_integral_image[i];
    }
    delete [] integral_image;
    delete [] squared_integral_image;

    // Rect window_frame(kuwahara_level/2, kuwahara_level/2, 
    //     output.cols-(kuwahara_level-1), output.rows-(kuwahara_level-1) );

    // Mat final_output=output(window_frame).clone();


    return output;
}