#include "ml_class.h"


Parent_ML::Parent_ML(){
}

bool Parent_ML::Read_Original_Image(string& full_path, Mat& output_ori_img){
    output_ori_img=imread(full_path,0);
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
            // cout<<"extracted_name_with_ext: "<<extracted_name_with_ext<<endl;
            // getchar();
            output_ori_img=imread(extracted_name_with_ext,0);
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

void Parent_ML::Main_Process(Machine_Learning_Data_Preparation *&prepared_data){
    ml=prepared_data;
    // confusion_matrix=new Mat[ml->k_fold_value];
    confusion_matrix_test=new Mat[ml->k_fold_value];
    confusion_matrix_train=new Mat[ml->k_fold_value];

    // accuracy=new float[ml->k_fold_value];
    accuracy_test=new float[ml->k_fold_value];
    accuracy_train=new float[ml->k_fold_value];

    result_buffer=new char[50];
    result_buffer_test=new char*[ml->k_fold_value];
    result_buffer_train=new char*[ml->k_fold_value];

    // final_result_buffer=new char[50];
    final_result_buffer_test=new char[50];
    final_result_buffer_train=new char[50];
    

    // for(int i=0;i<ml->k_fold_value;i++){
    //     result_buffer[i]=new char[50];
    // }
    for(int i=0;i<ml->k_fold_value;i++){
        result_buffer_test[i]=new char[50];
    }
    for(int i=0;i<ml->k_fold_value;i++){
        result_buffer_train[i]=new char[50];
    }

    // getchar();
    Intialize();
    // Calculate_Result();
    // Calculate_standard_deviation();

    cout<<"done read structure"<<endl;
    //virtual functions
    confusion_matrix_test=Calculate_Confusion_Matrices(0,ml);
    cout<<"done read structure"<<endl;
    accuracy_test=Calculate_Accuracies(confusion_matrix_test, ml);
    cout<<"done read structure"<<endl;
    sum_accuracy_test=Calculate_Sum_Accuracy(accuracy_test, ml);
    cout<<"done read structure"<<endl;

    cout<<"confusion_matrix_test: "<<confusion_matrix_test[0]<<endl;
    // getchar();
    confusion_matrix_train=Calculate_Confusion_Matrices(1,ml);
    accuracy_train=Calculate_Accuracies(confusion_matrix_train, ml);
    sum_accuracy_train=Calculate_Sum_Accuracy(accuracy_train, ml);
    
    //parent functions
    mean_test=Calculate_Mean(sum_accuracy_test, ml);
    variance_test=Calculate_Variance(accuracy_test, mean_test, ml);
    sta_dev_test=Calculate_Standard_Deviation(variance_test);

    mean_train=Calculate_Mean(sum_accuracy_train, ml);
    variance_train=Calculate_Variance(accuracy_train, mean_train, ml);
    sta_dev_train=Calculate_Standard_Deviation(variance_train);

    Return_Parameter();
    
    // getchar();


}

Parent_ML::~Parent_ML(){
    
    // for(int i=0;i<ml->k_fold_value;i++){
    //     delete result_buffer[i];
    // }

    for(int i=0;i<ml->k_fold_value;i++){
        delete result_buffer_test[i];
    }

    for(int i=0;i<ml->k_fold_value;i++){
        delete result_buffer_train[i];
    }

    delete result_buffer;
    // delete accuracy;
    delete accuracy_test;
    delete accuracy_train;

    // delete confusion_matrix;
    delete confusion_matrix_test;
    delete confusion_matrix_train;
}

float Parent_ML::Calculate_Mean(float i_sum_accuracy, Machine_Learning_Data_Preparation *i_ml){
    float out_mean=i_sum_accuracy/i_ml->k_fold_value;
    return out_mean;
}

float Parent_ML::Calculate_Variance(float i_accuracy[],float i_mean, Machine_Learning_Data_Preparation *i_ml){
    float out_variance=0;
    for(int j=0; j < i_ml->k_fold_value; j++){
        out_variance=out_variance+(i_accuracy[j]-i_mean)*(i_accuracy[j]-i_mean);
    }
    out_variance=out_variance/i_ml->k_fold_value;
    return out_variance;
}

float Parent_ML::Calculate_Standard_Deviation(float i_variance){
    float out_sta_dev=sqrt(i_variance);
    return out_sta_dev;
}

float Parent_ML::Accuracy_Calculation(const Mat& i_confusion_matrix){
    // load classifier from the specified file
    float out_accuracy;
    float total_accurate=i_confusion_matrix.at<int>(0,0)+i_confusion_matrix.at<int>(1,1);
    float total_number_of_values=i_confusion_matrix.at<int>(0,0)+i_confusion_matrix.at<int>(0,1)+
    i_confusion_matrix.at<int>(1,0)+i_confusion_matrix.at<int>(1,1);
    out_accuracy=total_accurate/total_number_of_values;


    float temp=i_confusion_matrix.at<int>(0,0)+i_confusion_matrix.at<int>(0,1);

    float precision_rodent=i_confusion_matrix.at<int>(0,0)/temp;
    cout<<"-----------------"<<endl;
    // cout<<"precision_rodent: "<<precision_rodent<<endl;
    printf("precision_rodent:%.2f\n",precision_rodent);



    float temp2=i_confusion_matrix.at<int>(0,0)+i_confusion_matrix.at<int>(1,0);
    float recall_rodent=i_confusion_matrix.at<int>(0,0)/temp2;
    // cout<<"recall_rodent: "<<recall_rodent<<endl;
    printf("recall_rodent:%.2f\n",recall_rodent);

    float f1=2*( (precision_rodent*recall_rodent)/(precision_rodent+recall_rodent) );
    // cout<<"F1 Value: "<<f1<<endl;
    printf("F1 Value:%.2f\n",f1);
    cout<<"-----------------"<<endl;

    return out_accuracy;
}


void Parent_ML::Return_Prev_and_Next_Full_File_Path(string input_name, string& prev_name, string& next_name){    

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


Mat Parent_ML::test_and_save_classifier(const Ptr<StatModel>& model,
                                     const Mat& data, const Mat& responses,
                                     int ntrain_samples, int rdelta,
                                     const string& filename_to_save, int ml_technique )
{
    // int nsamples_all = data.rows;
    int nsamples_all = ntrain_samples;
    double train_hr = 0, test_hr = 0;
    int training_correct_predict=0;
    // compute prediction error on training data
    // for(int i=0; i<nsamples_all; i++){
    Mat confusion_Matrix = Mat::zeros( 2, 2, CV_32S );
    
    // getchar();
    int problem_val=0;
    for(int i=0; i<nsamples_all; i++){
        Mat sample = data.row(i);
        // int actual_value=responses.at<int>(i)-48;
        int actual_value;
        if(ml_technique==1){
            actual_value=responses.at<int>(i);
        }else{
            actual_value=responses.at<float>(i);
        }
        // int actual_value=responses.at<int>(i);//this is work for tech 1
        // cout<<"actual_value: "<<responses.at<int>(i)<<endl;
        // getchar();
        // cout << "Actual_ha: " << responses.at<float>(i) << " row " << sample << endl;
        cout << "Actual: " << actual_value << endl;
        // cout<<"sample: "<<sample<<endl;
        float r = model->predict( sample );
        cout<<"predict: "<<r<<endl;
        // getchar();
        // cout << "Predict:  r = " << round(r) << endl;//rounding in case of random_forest
        // getchar();
        // int r_int=r;
        int r_int=(int)round(r);//random forrest case
        // if( r_int == actual_value ){ //prediction is correct
        if( r_int == actual_value ){ //prediction is correct
            training_correct_predict++;
        }
        confusion_Matrix.at<int>(actual_value,r_int)=confusion_Matrix.at<int>(actual_value,r_int)+1;
        // if(actual_value==0){
        //     if(r>0.3){
        //         cout<<"This "<<r<<" value is problem"<<endl;
        //         problem_val++;
        //     }else{

        //     }
        // }else{
        //     if(r<0.7){
        //         cout<<"This "<<r<<" value is problem"<<endl;
        //         problem_val++;
        //     }else{
                
        //     }
        // }
        


        // cout<<"confusion_Matrix: "<<confusion_Matrix<<endl;
        // getchar();


        // cout << "training_correct_predict = " << training_correct_predict << endl;
        // getchar();

        // cout << "Sample: " << responses.at<int>(i) << " row " << data.row(i) << endl;
        // float r = model->predict( sample );
        // cout << "Predict:  r = " << r << endl;
        // if( (int)r == (int)(responses.at<int>(i)) ) //prediction is correct
        //     training_correct_predict++;
   
    // r = std::abs(r + rdelta - responses.at<int>(i)) <= FLT_EPSILON ? 1.f : 0.f;
    
     
        //if( i < ntrain_samples )
        //    train_hr += r;
        //else
        //    test_hr += r;

    }

    //test_hr /= nsamples_all - ntrain_samples;
    //train_hr = ntrain_samples > 0 ? train_hr/ntrain_samples : 1.;
    printf("ntrain_samples %d training_correct_predict %d \n",ntrain_samples, training_correct_predict);
    // *accuracy=training_correct_predict*100.0/ntrain_samples;
    // getchar();
    if( filename_to_save.empty() )  {
        printf( "\nTest Recognition rate: training set = %.1f%% \n\n", training_correct_predict*100.0/ntrain_samples);

    }
    // if( filename_to_save.empty() )  printf( "\nTest Recognition rate: training set = %.1f%% \n\n", *accuracy);


    if( !filename_to_save.empty() )
    {
        model->save( filename_to_save );
    }
/*************   Example of how to predict a single sample ************************/   
// Use that for the assignment3, for every frame after computing the features, r is the prediction given the features listed in this format
    //Mat sample = data.row(i);
//     Mat sample1 = (Mat_<float>(1,9) << 1.52101, 13.64, 4.4899998, 1.1, 71.779999, 0.059999999, 8.75, 0, 0);// 1
//     float r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
//     sample1 = (Mat_<float>(1,9) << 1.518, 13.71, 3.9300001, 1.54, 71.809998, 0.54000002, 8.21, 0, 0.15000001);//2
//     r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
//     sample1 = (Mat_<float>(1,9) << 1.51694,12.86,3.58,1.31,72.61,0.61,8.79,0,0);//3
//     r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
// //    sample1 = (Mat_<float>(1,9) << );//4
// //    r = model->predict( sample1 );
// //    cout << "Prediction: " << r << endl;
//     sample1 = (Mat_<float>(1,9) << 1.5151401, 14.01, 2.6800001, 3.5, 69.889999, 1.6799999, 5.8699999, 2.2, 0);//5
//     r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
//     sample1 = (Mat_<float>(1,9) << 1.51852, 14.09, 2.1900001, 1.66, 72.669998, 0, 9.3199997, 0, 0);//6
//     r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
//     sample1 = (Mat_<float>(1,9) << 1.51131,13.69,3.2,1.81,72.81,1.76,5.43,1.19,0);//7
//     r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
    cout<<"problem_val: "<<problem_val<<endl;
    // getchar();
    return confusion_Matrix;
    
/**********************************************************************/
}

Mat Parent_ML::Advanced_test_and_save_classifier2(const Ptr<StatModel>& model,
                                     const Mat& data, const Mat& responses, const Mat& data_contours,
                                     int ntrain_samples, int rdelta,
                                     const string& filename_to_save, int ml_technique, bool recall_save )
{
    // int nsamples_all = data.rows;

    int nsamples_all = ntrain_samples;
    double train_hr = 0, test_hr = 0;
    int training_correct_predict=0;
    // compute prediction error on training data
    // for(int i=0; i<nsamples_all; i++){
    Mat confusion_Matrix = Mat::zeros( 2, 2, CV_32S );



    ofstream recall_rodent;
    ofstream recall_bird;
    // getchar();
    if(recall_save==true){
        recall_rodent.open("recall_rodent.txt");
        // recall_bird.open("recall_bird.txt");
        
        recall_rodent<<"Index,TP,FP,Precision,Recall \n";
        // recall_bird<<"Index,TP,FP,Precision,Recall \n";
    }
    

    int problem_val=0;
    for(int i=0; i<nsamples_all; i++){
        Mat sample = data.row(i);
        // Mat sample = data_contours.row(i);
        // cout<<"sample: "<<sample<<endl;
        // int actual_value=responses.at<int>(i)-48;
        int actual_value;
        if((ml_technique==1)){
            actual_value=responses.at<int>(i);
        }else{
            actual_value=responses.at<float>(i);
        }
        // cout<<"there"<<endl;
        // int actual_value=responses.at<int>(i);//this is work for tech 1
        // cout<<"actual_value: "<<responses.at<int>(i)<<endl;
        // getchar();
        // cout << "Actual_ha: " << responses.at<float>(i) << " row " << sample << endl;
        // cout << "Actual: " << actual_value << endl;
        // cout<<"sample: "<<sample<<endl;
        float r = model->predict( sample );

        // cout<<"predict: "<<r<<endl;
        // getchar();
        // cout << "Predict:  r = " << round(r) << endl;//rounding in case of random_forest
        // getchar();
        // int r_int=r;
        // cout<<"r: "<<r<<endl;
        // cout<<"actual_value: "<<actual_value<<endl;
        // getchar();
        int r_int;
        if(ml_technique==3){
            if(r!=0){
                // cout<<"r: "<<r<<endl;
                // getchar();
                r_int=1;
            }else{
                r_int=0;//random forrest case    
            }
        }else{
            r_int=(int)round(r);//random forrest case    
        }
        

        // cout<<"r_int: "<<r_int<<endl;
        // if( r_int == actual_value ){ //prediction is correct
        if( r_int == actual_value ){ //prediction is correct
            training_correct_predict++;
        }
        
        confusion_Matrix.at<int>(actual_value,r_int)=confusion_Matrix.at<int>(actual_value,r_int)+1;
        
        if(recall_save==true){
            recall_rodent<<i;
            recall_rodent<<" ";
            recall_rodent<<to_string( confusion_Matrix.at<int>(0,0) );
            recall_rodent<<" ";
            recall_rodent<<to_string( confusion_Matrix.at<int>(0,1) );
            recall_rodent<<" ";
            if(confusion_Matrix.at<int>(0,0)==0){
                recall_rodent<<"0";    
            }else{
                recall_rodent<<to_string( confusion_Matrix.at<float>(0,0)/(confusion_Matrix.at<float>(0,0)+confusion_Matrix.at<float>(0,1) ) );    
            }
            
            recall_rodent<<"\n";

            // recall_bird<<i;
            // recall_bird<<" ";
            // recall_bird<<to_string( confusion_Matrix.at<int>(1,1) );
            // recall_bird<<" ";
            // recall_bird<<to_string( confusion_Matrix.at<int>(1,0) );
            // recall_bird<<" ";
            // if(confusion_Matrix.at<int>(1,1)==0){
            //     recall_bird<<"0";
            // }else{
            //     recall_bird<<to_string( confusion_Matrix.at<float>(1,1)/(confusion_Matrix.at<float>(1,1)+confusion_Matrix.at<float>(1,0) ) );
            // }
            
            // recall_bird<<"\n";
        }
        

        // if(actual_value==0){
        //     if(r>0.3){
        //         cout<<"This "<<r<<" value is problem"<<endl;
        //         problem_val++;
        //     }else{

        //     }
        // }else{
        //     if(r<0.7){
        //         cout<<"This "<<r<<" value is problem"<<endl;
        //         problem_val++;
        //     }else{
                
        //     }
        // }
        


        // cout<<"confusion_Matrix: "<<confusion_Matrix<<endl;
        // getchar();


        // cout << "training_correct_predict = " << training_correct_predict << endl;
        // getchar();

        // cout << "Sample: " << responses.at<int>(i) << " row " << data.row(i) << endl;
        // float r = model->predict( sample );
        // cout << "Predict:  r = " << r << endl;
        // if( (int)r == (int)(responses.at<int>(i)) ) //prediction is correct
        //     training_correct_predict++;
   
    // r = std::abs(r + rdelta - responses.at<int>(i)) <= FLT_EPSILON ? 1.f : 0.f;
    
     
        //if( i < ntrain_samples )
        //    train_hr += r;
        //else
        //    test_hr += r;

    }
    recall_rodent.close();
    // recall_bird.close();

    ifstream i_recall_rodent2, i_recall_bird2;
    i_recall_rodent2.open("recall_rodent.txt");
    // i_recall_bird2.open("recall_bird.txt");

    ofstream recall_bird_final, recall_rodent_final;
    
    if(recall_save==true){
        recall_rodent_final.open("recall_rodent_final.txt");
        // recall_bird_final.open("recall_bird_final.txt");
        
        recall_rodent_final<<"Index,TP,FP,Precision,Recall \n";
        // recall_bird_final<<"Index,TP,FP,Precision,Recall \n";

        string read_line;
        int k=0;
        
        if(i_recall_rodent2.is_open()){
            while(getline(i_recall_rodent2, read_line)){
                if(k==0){

                }else{
                    recall_rodent_final<<read_line;
                    int delimiter=0;
                    string name;
                    delimiter = read_line.find(' ');
                    string temp_line=read_line.substr(delimiter+1);
                    delimiter = temp_line.find(' ');
                    temp_line=temp_line.substr(0,delimiter);
                    
                    recall_rodent_final<<" ";
                    recall_rodent_final<<to_string( stof(temp_line)/(confusion_Matrix.at<int>(0,0)+confusion_Matrix.at<int>(1,0) ));
                    recall_rodent_final<<"\n";
                }
                
                // cout<<"name: "<<name<<endl;
                k++;
            }
        }
        recall_rodent_final.close();
        // recall_bird_final.close();
    }


    

    //test_hr /= nsamples_all - ntrain_samples;
    //train_hr = ntrain_samples > 0 ? train_hr/ntrain_samples : 1.;
    printf("ntrain_samples %d training_correct_predict %d \n",ntrain_samples, training_correct_predict);
    // *accuracy=training_correct_predict*100.0/ntrain_samples;
    // getchar();
    if( filename_to_save.empty() )  {
        printf( "\nTest Recognition rate: training set = %.1f%% \n\n", training_correct_predict*100.0/ntrain_samples);

    }
    // if( filename_to_save.empty() )  printf( "\nTest Recognition rate: training set = %.1f%% \n\n", *accuracy);


    if( !filename_to_save.empty() )
    {
        model->save( filename_to_save );

    }
/*************   Example of how to predict a single sample ************************/   
// Use that for the assignment3, for every frame after computing the features, r is the prediction given the features listed in this format
    //Mat sample = data.row(i);
//     Mat sample1 = (Mat_<float>(1,9) << 1.52101, 13.64, 4.4899998, 1.1, 71.779999, 0.059999999, 8.75, 0, 0);// 1
//     float r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
//     sample1 = (Mat_<float>(1,9) << 1.518, 13.71, 3.9300001, 1.54, 71.809998, 0.54000002, 8.21, 0, 0.15000001);//2
//     r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
//     sample1 = (Mat_<float>(1,9) << 1.51694,12.86,3.58,1.31,72.61,0.61,8.79,0,0);//3
//     r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
// //    sample1 = (Mat_<float>(1,9) << );//4
// //    r = model->predict( sample1 );
// //    cout << "Prediction: " << r << endl;
//     sample1 = (Mat_<float>(1,9) << 1.5151401, 14.01, 2.6800001, 3.5, 69.889999, 1.6799999, 5.8699999, 2.2, 0);//5
//     r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
//     sample1 = (Mat_<float>(1,9) << 1.51852, 14.09, 2.1900001, 1.66, 72.669998, 0, 9.3199997, 0, 0);//6
//     r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
//     sample1 = (Mat_<float>(1,9) << 1.51131,13.69,3.2,1.81,72.81,1.76,5.43,1.19,0);//7
//     r = model->predict( sample1 );
//     cout << "Prediction: " << r << endl;
    cout<<"problem_val: "<<problem_val<<endl;
    // getchar();
    return confusion_Matrix;
    
/**********************************************************************/
}
float *Child_ML<ANN_MLP>::Calculate_Accuracies(Mat *&i_confusion_matrix, Machine_Learning_Data_Preparation *&i_ml){
    float *out_accuracies;
    out_accuracies=new float[i_ml->k_fold_value];

    for(int i=0; i< i_ml->k_fold_value; i++){
        out_accuracies[i]=Accuracy_Calculation(i_confusion_matrix[i]);
    }
    return out_accuracies;
}

Mat *Child_ML<ANN_MLP>::Calculate_Confusion_Matrices(int type, Machine_Learning_Data_Preparation *&i_ml){
    Mat *out_confusion_matrix;
    out_confusion_matrix=new Mat[i_ml->k_fold_value];

    for(int i=0; i< i_ml->k_fold_value; i++){
        bool recall_save=false;
        if(type==0){//test
            // out_confusion_matrix[i]=test_and_save_classifier(model[i], i_ml->test_data[i], i_ml->test_responses_int[i], i_ml->ntest_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
            
            if(i==0){
                recall_save=true;
            }

            out_confusion_matrix[i]=Advanced_test_and_save_classifier2(model[i], i_ml->test_data[i], i_ml->test_responses_int[i], i_ml->CEs_test_data[i], i_ml->ntest_samples, 0, i_ml->filename_to_save, i_ml->ml_technique,recall_save);    
            recall_save=false;
            
            // out_confusion_matrix[i]=Advanced_Test_and_Save_Classifier(model[i], i_ml->test_data[i], i_ml->test_responses_int[i], i_ml->imgname, i_ml->img_root ,i_ml->ntest_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
        }else{//train
            // out_confusion_matrix[i]=test_and_save_classifier(model[i], i_ml->train_data[i], i_ml->train_responses_int[i], i_ml->ntrain_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
            out_confusion_matrix[i]=Advanced_test_and_save_classifier2(model[i], i_ml->train_data[i], i_ml->train_responses_int[i], i_ml->CEs_train_data[i], i_ml->ntrain_samples, 0, i_ml->filename_to_save, i_ml->ml_technique, recall_save);
            // out_confusion_matrix[i]=Advanced_Test_and_Save_Classifier(model[i], i_ml->train_data[i], i_ml->train_responses_int[i], i_ml->imgname, i_ml->img_root ,i_ml->ntrain_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
        }
        
    }
    return out_confusion_matrix;
}

float Child_ML<ANN_MLP>::Calculate_Sum_Accuracy(float *&i_accuracy, Machine_Learning_Data_Preparation *&i_ml){
    float out_sum_accuracy=0;
    for(int i=0; i< i_ml->k_fold_value; i++){
        out_sum_accuracy=out_sum_accuracy+i_accuracy[i];
        // Return_Parameter(i);
    }
    return out_sum_accuracy;
}

void Child_ML<ANN_MLP>::Intialize(){
    system_clock::time_point start = system_clock::now();
    // double p_method_param, int p_max_iter
    cout<<"start initialize ANN"<<endl;
    // cout<<"ml->k_fold_value: "<<ml->k_fold_value<<endl;
    model=new Ptr<ANN_MLP>[ml->k_fold_value];
    
    cout<<"ml->the_number_of_data: "<<ml->the_number_of_data<<endl;
    int layer_sz[] = { ml->the_number_of_data, 100, 100, ml->class_count };
    cout <<  " sizeof layer_sz " << sizeof(layer_sz) << " sizeof layer_sz[0]) " << sizeof(layer_sz[0]) << endl;
    int nlayers = (int)(sizeof(layer_sz)/sizeof(layer_sz[0]));
    cout << " nlayers  " << nlayers << endl;    
    Mat layer_sizes( 1, nlayers, CV_32S, layer_sz );
    // cout<<"ml->tdata[i]: "<<ml->tdata[0]<<endl;
    for(int i=0;i<ml->k_fold_value;i++){
        // cout<<"ml->tdata[i]:"<<ml->tdata[i]<<endl;
        cout << "iteration ("<<i<<") Training the classifier (may take a few minutes)...\n";
        // getchar();
        model[i] = ANN_MLP::create();
        model[i]->setLayerSizes(layer_sizes);
        model[i]->setActivationFunction(a_function, 0, 0);
        // model->setActivationFunction(ANN_MLP::IDENTITY, 0, 0);
        model[i]->setTermCriteria(ml->TC(max_iter,method_param));
        model[i]->train(ml->tdata[i]);
        // model[i]->save( "weight/NN.xml" );
        
    }
            // test_and_save_classifier(model_final, main_data, main_responses, data.rows, 0, path_save, ml_technique);

    system_clock::time_point end = system_clock::now();
    double seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    cout<<"time: "<<seconds<<" sec"<<endl;
    // getchar();
    cout << endl;
}

void Child_ML<ANN_MLP>::Calculate_Result(){
    // for(int i=0;i<ml->k_fold_value;i++){
    //     confusion_matrix[i]=test_and_save_classifier(model[i], ml->test_data[i], ml->test_responses_int[i], ml->ntest_samples, 0, ml->filename_to_save,ml->ml_technique);
    //     accuracy[i]=Accuracy_Calculation(confusion_matrix[i]);
    //     sum_accuracy=sum_accuracy+accuracy[i];
    //     Return_Parameter(i);
    // }
}

void Child_ML<ANN_MLP>::Return_Parameter(){
    sprintf(result_buffer, "%d, %d, %lf, %d, %f, %f \n", t_method, a_function, method_param, max_iter, mean_test*100, mean_train*100);  //header
    // sprintf(result_buffer[index], "%d, %d, %d, %lf, %d, %d, %f \n", index, t_method, a_function, method_param, max_iter, ml->class_count, accuracy[index]);  //header
}

string Child_ML<ANN_MLP>::Head_Parameter(){
    return "Index, Method_Type, a_function, MethodParameter, MaxIteration, ClassCount, Test_Accuracy, Train_Accuracy ";
}

void Child_ML<ANN_MLP>::Create_Weight(string path_save){
    system_clock::time_point start = system_clock::now();
    // double p_method_param, int p_max_iter
    cout<<"start initialize ANN"<<endl;
    // cout<<"ml->k_fold_value: "<<ml->k_fold_value<<endl;
    
    cout<<"ml->the_number_of_data: "<<ml->the_number_of_data<<endl;
    int layer_sz[] = { ml->the_number_of_data, 100, 100, ml->class_count };
    cout <<  " sizeof layer_sz " << sizeof(layer_sz) << " sizeof layer_sz[0]) " << sizeof(layer_sz[0]) << endl;
    int nlayers = (int)(sizeof(layer_sz)/sizeof(layer_sz[0]));
    cout << " nlayers  " << nlayers << endl;    
    Mat layer_sizes( 1, nlayers, CV_32S, layer_sz );
    
    
    cout<<" Training the classifier (may take a few minutes)...\n"<<endl;
    model_final = ANN_MLP::create();
    model_final->setLayerSizes(layer_sizes);
    model_final->setActivationFunction(a_function, 0, 0);
    // model->setActivationFunction(ANN_MLP::IDENTITY, 0, 0);
    model_final->setTermCriteria(ml->TC(max_iter,method_param));
    // model_final->train(ml->tdata_for_all);
    
    system_clock::time_point end = system_clock::now();
    double seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    cout<<"time: "<<seconds<<" sec"<<endl;
    // getchar();
    cout << endl;
    test_and_save_classifier(model_final, ml->data, ml->main_responses_int, ml->n_all_samples, 0, path_save, ml->ml_technique);
}


void Child_ML<Boost>::Intialize(){
    model=new Ptr<Boost>[ml->k_fold_value];
    system_clock::time_point start = system_clock::now();
    for(int i=0;i<ml->k_fold_value;i++){
        cout << "iteration ("<<i<<") Training the classifier (may take a few minutes)...\n";
        model[i]=Boost::create();
        model[i]->setBoostType(boost_type);  //Gentle 0.5 and true{DISCRETE, REAL, LOGIT, GENTLE}
        model[i]->setWeakCount(weak_count);       //the Gentle best=98;
        model[i]->setWeightTrimRate(weight_trim_rate);//the Gentle best=0.83;
        model[i]->setMaxDepth(max_depth);         //the Gentle best=2;
        model[i]->setUseSurrogates(false);
        model[i]->setPriors(Mat()); 
        cout << "Training the classifier (may take a few minutes)...\n";
        model[i]->train(ml->tdata[i]);

    }
    /*Caculate performance of program*/
    system_clock::time_point end = system_clock::now();
    double seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    cout<<"time: "<<seconds<<" sec"<<endl;
    // getchar();
    cout << endl;
}


Mat *Child_ML<Boost>::Calculate_Confusion_Matrices(int type, Machine_Learning_Data_Preparation *&i_ml){
    Mat *out_confusion_matrix;
    out_confusion_matrix=new Mat[i_ml->k_fold_value];

    for(int i=0; i< i_ml->k_fold_value; i++){
        bool recall_save=false;

        if(type==0){//test
            // out_confusion_matrix[i]=test_and_save_classifier(model[i], i_ml->test_data[i], i_ml->test_responses_int[i], i_ml->ntest_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
            if(i==0){
                recall_save=true;
            }
            out_confusion_matrix[i]=Advanced_test_and_save_classifier2(model[i], i_ml->test_data[i], i_ml->test_responses_int[i], i_ml->CEs_test_data[i], i_ml->ntest_samples, 0, i_ml->filename_to_save, i_ml->ml_technique,recall_save);
            recall_save=false;
            // out_confusion_matrix[i]=Advanced_Test_and_Save_Classifier(model[i], i_ml->test_data[i], i_ml->test_responses_int[i], i_ml->imgname, i_ml->img_root ,i_ml->ntest_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
        }else{//train
            // out_confusion_matrix[i]=test_and_save_classifier(model[i], i_ml->train_data[i], i_ml->train_responses_int[i], i_ml->ntrain_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
            out_confusion_matrix[i]=Advanced_test_and_save_classifier2(model[i], i_ml->train_data[i], i_ml->train_responses_int[i], i_ml->CEs_train_data[i], i_ml->ntrain_samples, 0, i_ml->filename_to_save, i_ml->ml_technique,recall_save);
            // out_confusion_matrix[i]=Advanced_Test_and_Save_Classifier(model[i], i_ml->train_data[i], i_ml->train_responses_int[i], i_ml->imgname, i_ml->img_root ,i_ml->ntrain_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
        }
    }
    return out_confusion_matrix;
}

float *Child_ML<Boost>::Calculate_Accuracies(Mat *&i_confusion_matrix, Machine_Learning_Data_Preparation *&i_ml){
    float *out_accuracies;
    out_accuracies=new float[i_ml->k_fold_value];

    for(int i=0; i< i_ml->k_fold_value; i++){
        out_accuracies[i]=Accuracy_Calculation(i_confusion_matrix[i]);
    }
    return out_accuracies;
}

float Child_ML<Boost>::Calculate_Sum_Accuracy(float *&i_accuracy, Machine_Learning_Data_Preparation *&i_ml){
    float out_sum_accuracy=0;
    for(int i=0; i< i_ml->k_fold_value; i++){
        out_sum_accuracy=out_sum_accuracy+i_accuracy[i];
        // Return_Parameter();
    }
    return out_sum_accuracy;
}

void Child_ML<Boost> ::Calculate_Result(){
    // for(int i=0;i<ml->k_fold_value;i++){
    //     confusion_matrix[i]=test_and_save_classifier(model[i], ml->test_data[i], ml->test_responses_int[i], ml->ntest_samples, 0, ml->filename_to_save,ml->ml_technique);
    //     accuracy[i]=Accuracy_Calculation(confusion_matrix[i]);
    //     sum_accuracy=sum_accuracy+accuracy[i];
    //     Return_Parameter(i);
    // }
}


void Child_ML<Boost>::Return_Parameter(){
    sprintf(result_buffer, "%d, %d, %f, %d, %f, %f \n", boost_type, weak_count, weight_trim_rate, max_depth, mean_test*100, mean_train*100);  //header
    // sprintf(result_buffer, "%d, %d, %f, %d, %d, %f, %f\n", max_depth, min_sample_count, regression_accuracy, tc_value, ml->class_count, mean_test*100, mean_train*100);  //header
    // sprintf(result_buffer, "%d, %d, %f, %d, %d, %f, %f\n", max_depth, min_sample_count, regression_accuracy, tc_value, ml->class_count, mean_test*100, mean_train*100);  //header
}

string Child_ML<Boost>::Head_Parameter(){
    return "BoostType, WeakCount, WeightTrimRate, ClassCount, Test_Accuracy, Train_Accuracy";
}

void Child_ML<Boost>::Create_Weight(string path_save){
    system_clock::time_point start = system_clock::now();
    
    cout <<" Training the classifier (may take a few minutes)...\n"<<endl;;
    model_final=Boost::create();
    model_final->setBoostType(boost_type);  //Gentle 0.5 and true{DISCRETE, REAL, LOGIT, GENTLE}
    model_final->setWeakCount(weak_count);       //the Gentle best=98;
    model_final->setWeightTrimRate(weight_trim_rate);//the Gentle best=0.83;
    model_final->setMaxDepth(max_depth);         //the Gentle best=2;
    model_final->setUseSurrogates(false);
    model_final->setPriors(Mat());
    model_final->train(ml->tdata_for_all);
    
    /*Caculate performance of program*/
    system_clock::time_point end = system_clock::now();
    double seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    cout<<"time: "<<seconds<<" sec"<<endl;
    // getchar();
    cout << endl;
    test_and_save_classifier(model_final, ml->data, ml->main_responses_int, ml->n_all_samples, 0, path_save, ml->ml_technique);
}

void Child_ML<RTrees>::Intialize(){
    // sprintf(final_result_buffer, "%d, %d, %f, %d, %d", max_depth, min_sample_count, regression_accuracy, tc_value,ml->class_count);  //header
    // cout<<"THis is RF"<<endl;
    // getchar();

    model=new Ptr<RTrees>[ml->k_fold_value];
    // model->load( filename_to_save );
    system_clock::time_point start = system_clock::now();
    for(int i=0;i<ml->k_fold_value;i++){
        model[i] = RTrees::create();
        model[i]->setMaxDepth(max_depth);
        model[i]->setMinSampleCount(min_sample_count);
        model[i]->setRegressionAccuracy(regression_accuracy);
        model[i]->setUseSurrogates(false /* true */);
        model[i]->setMaxCategories(2);
        model[i]->setTermCriteria(ml->TC(tc_value,0));
        // model[i]->setTermCriteria(CV_TERMCRIT_ITER |    CV_TERMCRIT_EPS);
        cout << "Training the classifier (may take a few minutes)...\n";
        // model[i]->setTermCriteria(TermCriteria(TermCriteria::COUNT, 50, 0));
        model[i]->train(ml->tdata[i]);
    }
     /*Caculate performance of program*/
    system_clock::time_point end = system_clock::now();
    double seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    cout<<"time: "<<seconds<<" sec"<<endl;
    // getchar();
    // fps = 1000000/seconds;

    // cout<<"tc_value: "<<tc_value<<endl;
    // getchar();
    cout << endl;
}

float *Child_ML<RTrees>::Calculate_Accuracies(Mat *&i_confusion_matrix, Machine_Learning_Data_Preparation *&i_ml){
    float *out_accuracies;
    out_accuracies=new float[i_ml->k_fold_value];

    for(int i=0; i< i_ml->k_fold_value; i++){
        out_accuracies[i]=Accuracy_Calculation(i_confusion_matrix[i]);
    }
    return out_accuracies;
}

Mat *Child_ML<RTrees>::Calculate_Confusion_Matrices(int type, Machine_Learning_Data_Preparation *&i_ml){
    Mat *out_confusion_matrix;
    out_confusion_matrix=new Mat[i_ml->k_fold_value];

    for(int i=0; i< i_ml->k_fold_value; i++){
        bool recall_save=false;
        if(type==0){//test
            // out_confusion_matrix[i]=test_and_save_classifier(model[i], i_ml->test_data[i], i_ml->test_responses_int[i], i_ml->ntest_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
            if(i==0){
                recall_save=true;
            }
            out_confusion_matrix[i]=Advanced_test_and_save_classifier2(model[i], i_ml->test_data[i], i_ml->test_responses_int[i], i_ml->CEs_test_data[i], i_ml->ntest_samples, 0, i_ml->filename_to_save, i_ml->ml_technique,recall_save);
            recall_save=false;

            // out_confusion_matrix[i]=Advanced_Test_and_Save_Classifier(model[i], i_ml->test_data[i], i_ml->test_responses_int[i], i_ml->imgname, i_ml->img_root ,i_ml->ntest_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
        }else{//train
            // out_confusion_matrix[i]=test_and_save_classifier(model[i], i_ml->train_data[i], i_ml->train_responses_int[i], i_ml->ntrain_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
            out_confusion_matrix[i]=Advanced_test_and_save_classifier2(model[i], i_ml->train_data[i], i_ml->train_responses_int[i], i_ml->CEs_train_data[i], i_ml->ntrain_samples, 0, i_ml->filename_to_save, i_ml->ml_technique,recall_save);
            // out_confusion_matrix[i]=Advanced_Test_and_Save_Classifier(model[i], i_ml->train_data[i], i_ml->train_responses_int[i], i_ml->imgname, i_ml->img_root, i_ml->ntrain_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
        }
        
    }
    return out_confusion_matrix;
}

float Child_ML<RTrees>::Calculate_Sum_Accuracy(float *&i_accuracy, Machine_Learning_Data_Preparation *&i_ml){
    float out_sum_accuracy=0;
    for(int i=0; i< i_ml->k_fold_value; i++){
        out_sum_accuracy=out_sum_accuracy+i_accuracy[i];
        // Return_Parameter(i);
    }
    return out_sum_accuracy;
}

void Child_ML<RTrees>::Calculate_Result(){
    // for(int i=0;i<ml->k_fold_value;i++){
    //     confusion_matrix[i]=test_and_save_classifier(model[i], ml->test_data[i], ml->test_responses_int[i], ml->ntest_samples, 0, ml->filename_to_save,ml->ml_technique);
    //     // confusion_matrix[i]=test_and_save_classifier(model[i], ml->train_data[i], ml->train_responses_int[i], ml->ntrain_samples, 0, ml->filename_to_save,ml->ml_technique);
    //     accuracy[i]=Accuracy_Calculation(confusion_matrix[i]);
    //     sum_accuracy=sum_accuracy+accuracy[i];
    //     Return_Parameter(i);
    // }
}
void Child_ML<RTrees>::Return_Parameter(){
    // cout<<"h"<<endl;
    sprintf(result_buffer, "%d, %d, %f, %d, %d, %f, %f\n", max_depth, min_sample_count, regression_accuracy, tc_value, ml->class_count, mean_test*100, mean_train*100);  //header
    // sprintf(result_buffer[index], "%d, %d, %d, %f, %d, %d, %f \n", index, max_depth, min_sample_count, regression_accuracy, tc_value,ml->class_count);  //header
}

string Child_ML<RTrees>::Head_Parameter(){
    return "MaxDepth, RegressionAccuracy, MaxCategories, TermCritera, ClassCount, Test_Accuracy, Train_Accuracy";
}

void Child_ML<RTrees>::Create_Weight(string path_save){
    // model->load( filename_to_save );
    // cout<<"RTRee"<<endl;
    system_clock::time_point start = system_clock::now();
    model_final = RTrees::create();
    model_final->setMaxDepth(max_depth);
    model_final->setMinSampleCount(min_sample_count);
    model_final->setRegressionAccuracy(regression_accuracy);
    model_final->setUseSurrogates(false /* true */);
    model_final->setMaxCategories(2);
    model_final->setTermCriteria(ml->TC(tc_value,0));

    
    // model_final->setTermCriteria(CV_TERMCRIT_ITER |    CV_TERMCRIT_EPS);
    cout << "Training the classifier (may take a few minutes)...\n";
    // model_final->setTermCriteria(TermCriteria(TermCriteria::COUNT, 50, 0));
    // model_final->train(ml->tdata_for_all[0]);
    cout<<"done"<<endl;
    // model_final->train(ml->tdata[0]);
    
     /*Caculate performance of program*/
    system_clock::time_point end = system_clock::now();
    double seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    cout<<"time: "<<seconds<<" sec"<<endl;
    // getchar();
    // fps = 1000000/seconds;

    // cout<<"tc_value: "<<tc_value<<endl;
    // getchar();
    cout << endl;
    // test_and_save_classifier(model_final, ml->main_data, ml->main_responses_int, ml->n_all_samples, 0, path_save, ml->ml_technique);
}


void Child_ML<SVM>::Intialize(){
    // sprintf(final_result_buffer, "%d, %d, %f, %d, %d", max_depth, min_sample_count, regression_accuracy, tc_value,ml->class_count);  //header
    // cout<<"THis is RF"<<endl;
    // getchar();
    system_clock::time_point start = system_clock::now();
    model=new Ptr<SVM>[ml->k_fold_value];
    
    for(int i=0;i<ml->k_fold_value;i++){
        // cout<<"he"<<endl;
        model[i] = SVM::create();
        model[i]->setType(SVM::C_SVC);
        model[i]->setKernel(SVM::LINEAR);
        // model[i]->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 1000, 0.000001));
        // model[i] = SVM::create();
        // model[i]->setType(SVM::C_SVC);//C_SVC, NU_SVC, ONE_CLASS, EPS_SVR, NU_SVR
        // model[i]->setKernel(SVM::SIGMOID);//CUSTOM, LINEAR, POLY, RBF, SIGMOID, CHI2, INTER
        model[i]->setDegree(0);
        model[i]->setGamma(0);
        model[i]->setCoef0(0.0);
        model[i]->setC(10);
        model[i]->setNu(0);
        model[i]->setP(10);
        model[i]->setClassWeights(Mat());
        model[i]->setTermCriteria(ml->TC(100000000,0.0001));
        // model[i]->setTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1000, 0.000001);
        cout << "Training the classifier (may take a few minutes)...\n";
        // model[i]->setTermCriteria(TermCriteria(TermCriteria::COUNT, 50, 0));
        model[i]->train(ml->tdata[i]);
        // model[i]->save( "weight/SVM.xml" );
        // getchar();
    }
    system_clock::time_point end = system_clock::now();
    double seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    cout<<"time: "<<seconds<<" sec"<<endl;
    // cout<<"tc_value: "<<tc_value<<endl;
    // getchar();
    cout << endl;
}

float *Child_ML<SVM>::Calculate_Accuracies(Mat *&i_confusion_matrix, Machine_Learning_Data_Preparation *&i_ml){
    float *out_accuracies;
    out_accuracies=new float[i_ml->k_fold_value];

    for(int i=0; i< i_ml->k_fold_value; i++){
        out_accuracies[i]=Accuracy_Calculation(i_confusion_matrix[i]);
    }
    return out_accuracies;
}

Mat *Child_ML<SVM>::Calculate_Confusion_Matrices(int type, Machine_Learning_Data_Preparation *&i_ml){
    Mat *out_confusion_matrix;
    out_confusion_matrix=new Mat[i_ml->k_fold_value];

    // out_confusion_matrix[i]=test_and_save_classifier(model[i], i_ml->test_data[i], i_ml->test_responses_int[i], i_ml->ntest_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);


    for(int i=0; i< i_ml->k_fold_value; i++){
        bool recall_save=false;
        if(type==0){//test
            // out_confusion_matrix[i]=test_and_save_classifier(model[i], i_ml->test_data[i], i_ml->test_responses_int[i], i_ml->ntest_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
            if(i==0){
                recall_save=true;
            }
            out_confusion_matrix[i]=Advanced_test_and_save_classifier2(model[i], i_ml->test_data[i], i_ml->test_responses_int[i], i_ml->CEs_test_data[i], i_ml->ntest_samples, 0, i_ml->filename_to_save, i_ml->ml_technique,recall_save);
            recall_save=false;
            // out_confusion_matrix[i]=Advanced_Test_and_Save_Classifier(model[i], i_ml->test_data[i], i_ml->test_responses_int[i], i_ml->imgname, i_ml->img_root ,i_ml->ntest_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);

        }else{//train
            // out_confusion_matrix[i]=test_and_save_classifier(model[i], i_ml->train_data[i], i_ml->train_responses_int[i], i_ml->ntrain_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
            out_confusion_matrix[i]=Advanced_test_and_save_classifier2(model[i], i_ml->train_data[i], i_ml->train_responses_int[i], i_ml->CEs_train_data[i], i_ml->ntrain_samples, 0, i_ml->filename_to_save, i_ml->ml_technique,recall_save);
            // out_confusion_matrix[i]=Advanced_Test_and_Save_Classifier(model[i], i_ml->train_data[i], i_ml->train_responses_int[i], i_ml->imgname, i_ml->img_root ,i_ml->ntrain_samples, 0, i_ml->filename_to_save, i_ml->ml_technique);
        }
    }
    return out_confusion_matrix;
}

float Child_ML<SVM>::Calculate_Sum_Accuracy(float *&i_accuracy, Machine_Learning_Data_Preparation *&i_ml){
    float out_sum_accuracy=0;
    for(int i=0; i< i_ml->k_fold_value; i++){
        out_sum_accuracy=out_sum_accuracy+i_accuracy[i];
        // Return_Parameter(i);
    }
    return out_sum_accuracy;
}

void Child_ML<SVM>::Calculate_Result(){
    // for(int i=0;i<ml->k_fold_value;i++){
    //     confusion_matrix[i]=test_and_save_classifier(model[i], ml->test_data[i], ml->test_responses_int[i], ml->ntest_samples, 0, ml->filename_to_save,ml->ml_technique);
    //     // confusion_matrix[i]=test_and_save_classifier(model[i], ml->train_data[i], ml->train_responses_int[i], ml->ntrain_samples, 0, ml->filename_to_save,ml->ml_technique);
    //     accuracy[i]=Accuracy_Calculation(confusion_matrix[i]);
    //     sum_accuracy=sum_accuracy+accuracy[i];
    //     Return_Parameter(i);
    // }
}
void Child_ML<SVM>::Return_Parameter(){
    // sprintf(result_buffer[index], "%d, %d, %d, %f, %d, %d, %f \n", index, max_depth, min_sample_count, regression_accuracy, tc_value,ml->class_count);  //header
    
}

string Child_ML<SVM>::Head_Parameter(){
    return "Index, MaxDepth, RegressionAccuracy, MaxCategories, TermCritera, ClassCount, Accuracy";
}

void Child_ML<SVM>::Create_Weight(string path_save){

}