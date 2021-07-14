#ifndef ML_CLASS
#define ML_CLASS
#include "main.h"
// #include "machine_learning_data_preparation.h"
class Machine_Learning_Data_Preparation;
class Parent_ML{
public:
    
    // Mat *confusion_matrix;
    Mat *confusion_matrix_test;
    Mat *confusion_matrix_train;

    // float *accuracy;
    float *accuracy_test;
    float *accuracy_train;
    // char **result_buffer;
    // float sum_accuracy=0;
    float sum_accuracy_test=0;
    float sum_accuracy_train=0;

    int ml_technique;
    // float mean;
    float mean_test;
    float mean_train;
    
    // float variance=0;
    float variance_test=0;
    float variance_train=0;
    
    // float sta_dev;
    float sta_dev_test;
    float sta_dev_train;

    char *result_buffer;
    char **result_buffer_test;
    char **result_buffer_train;

    // char *final_result_buffer;
    char *final_result_buffer_test;
    char *final_result_buffer_train;

    Machine_Learning_Data_Preparation *ml;
    Parent_ML();
    ~Parent_ML();
    Mat Original_Image_to_Kuwahara_Image(Mat input_gray);
    
    void Main_Process(Machine_Learning_Data_Preparation *&prepared_data);
    float Accuracy_Calculation(const Mat& confusion_matrix);
    bool Calculate_standard_deviation();
    float Calculate_Mean(float i_sum_accuracy, Machine_Learning_Data_Preparation *i_ml);
    float Calculate_Variance(float i_accuracy[],float i_mean, Machine_Learning_Data_Preparation *i_ml);
    float Calculate_Standard_Deviation(float i_variance);
    
    Mat test_and_save_classifier(const Ptr<StatModel>& model,const Mat& data, const Mat& responses, int ntrain_samples, int rdelta, const string& filename_to_save, int ml_technique);
    Mat Advanced_Test_and_Save_Classifier(const Ptr<StatModel>& model,
                                     const Mat& data, const Mat& responses, const vector<string> imgname,
                                     const string& file_root, int ntrain_samples, int rdelta,
                                     const string& filename_to_save, int ml_technique );
    Mat Advanced_test_and_save_classifier2(const Ptr<StatModel>& model,
                                     const Mat& data, const Mat& responses, const Mat& data_contours,
                                     int ntrain_samples, int rdelta,
                                     const string& filename_to_save, int ml_technique, bool recall_save );
    
    bool Read_Original_Image(string& full_path, Mat& output_ori_img);//Advanced_Test_and_Save_Classifier
    void Return_Prev_and_Next_Full_File_Path(string input_name, string& prev_name, string& next_name);
    


    virtual void Intialize()=0;
    virtual Mat *Calculate_Confusion_Matrices(int type, Machine_Learning_Data_Preparation *&i_ml)=0;
    virtual float *Calculate_Accuracies(Mat *&i_confusion_matrix, Machine_Learning_Data_Preparation *&i_ml)=0;
    virtual float Calculate_Sum_Accuracy(float *&i_accuracy, Machine_Learning_Data_Preparation *&i_ml)=0;
    virtual void Calculate_Result()=0;
    virtual void Return_Parameter()=0;
    virtual string Head_Parameter()=0;
    virtual void Create_Weight(string path_save)=0;
};

template <class T>
T *Creat_Default_ML_Class(){
    return new T();
}

template <class T>
T *Creat_ML_Class(int p1, int p2, float p3, int p4, int p5){
    return new T(p1, p2, p3, p4, p5);
}


template<class T>//base template before specialized
class Child_ML : public Parent_ML{
public:
    // Machine_Learning *ml;
    void Intialize(){cout<<"error choose different technique"<<endl;}
    void Return_Parameter(){cout<<"error"<<endl;}
    string Head_Parameter(){return "error This is default child";}

    Mat *Calculate_Confusion_Matrices(int type, Machine_Learning_Data_Preparation *&i_ml){
        Mat *out_mat;
        return out_mat;
    }
    float *Calculate_Accuracies(Mat *&i_confusion_matrix, Machine_Learning_Data_Preparation *&i_ml){
        float *out_float;
        return out_float;
    }
    float Calculate_Sum_Accuracy(float *&i_accuracy, Machine_Learning_Data_Preparation *&i_ml){
        return 0;
    }
    void Create_Weight(string path_save){cout<<"defaule function"<<endl;}
};

template<>
class Child_ML<ANN_MLP> : public Parent_ML{
public:
    Ptr<ANN_MLP> *model;
    Ptr<ANN_MLP> model_final;
    int t_method = ANN_MLP::BACKPROP;//default
    // int t_method = ANN_MLP::BACKPROP;//default
    // int t_method = 2;//default
    // int t_method = 1;//default
    int a_function = ANN_MLP::SIGMOID_SYM;//default
    // int a_function = 4;//default
    int max_iter=100;
    double method_param=0.1;

    Child_ML(){}
    Child_ML(int p1, int p2, float p3, int p4, int p5){
        max_iter=p1;
        method_param=p3;
    }
    ~Child_ML(){delete model;}
    void Intialize();
    void Calculate_Result();
    Mat *Calculate_Confusion_Matrices(int type, Machine_Learning_Data_Preparation *&i_ml);
    float *Calculate_Accuracies(Mat *&i_confusion_matrix, Machine_Learning_Data_Preparation *&i_ml);
    float Calculate_Sum_Accuracy(float *&i_accuracy, Machine_Learning_Data_Preparation *&i_ml);
    void Return_Parameter();
    string Head_Parameter();
    void Create_Weight(string path_save);
};


template<>
class Child_ML<Boost> : public Parent_ML{

private:
    int boost_type=0;//Gentle 0.5 and true{DISCRETE, REAL, LOGIT, GENTLE}
    int weak_count=100;
    float weight_trim_rate=80.83;
    int max_depth=10;
public:
    Ptr<Boost> *model;
    Ptr<Boost> model_final;
    Child_ML(){}
    Child_ML(int p1, int p2, float p3, int p4, int p5){
        boost_type=p1;//Gentle 0.5 and true{DISCRETE, REAL, LOGIT, GENTLE}
        weak_count=p2;
        weight_trim_rate=p3;
        max_depth=p4;
    }
    ~Child_ML(){delete model;}
    void Intialize();
    void Calculate_Result();
    Mat *Calculate_Confusion_Matrices(int type, Machine_Learning_Data_Preparation *&i_ml);
    float *Calculate_Accuracies(Mat *&i_confusion_matrix, Machine_Learning_Data_Preparation *&i_ml);
    float Calculate_Sum_Accuracy(float *&i_accuracy, Machine_Learning_Data_Preparation *&i_ml);
    void Return_Parameter();
    string Head_Parameter();
    void Create_Weight(string path_save);
};


template<>
class Child_ML<RTrees> : public Parent_ML{

private:
    int max_depth=12;
    int min_sample_count=5;
    float regression_accuracy=0.01f;
    int max_categories=2;
    int tc_value=10;

public:
    Ptr<RTrees> *model;
    Ptr<RTrees> model_final;
    Child_ML(){}
    Child_ML(int p1, int p2, float p3, int p4, int p5){
        max_depth=p1;
        min_sample_count=p2;
        regression_accuracy=p3;
        max_categories=p4;
        tc_value=p5;
    }
    ~Child_ML(){delete model;}
    void Intialize();
    void Calculate_Result();
    Mat *Calculate_Confusion_Matrices(int type, Machine_Learning_Data_Preparation *&i_ml);
    float *Calculate_Accuracies(Mat *&i_confusion_matrix, Machine_Learning_Data_Preparation *&i_ml);
    float Calculate_Sum_Accuracy(float *&i_accuracy, Machine_Learning_Data_Preparation *&i_ml);
    void Return_Parameter();
    string Head_Parameter();
    void Create_Weight(string path_save);
};

template<>
class Child_ML<SVM> : public Parent_ML{

private:
    // int max_depth=12;
    // int min_sample_count=5;
    // float regression_accuracy=0.01f;
    // int max_categories=2;
    // int tc_value=100;

public:
    Ptr<SVM> *model;
    Child_ML(){}
    Child_ML(int p1, int p2, float p3, int p4, int p5){
        // max_depth=p1;
        // min_sample_count=p2;
        // regression_accuracy=p3;
        // max_categories=p4;
        // tc_value=p5;
    }
    ~Child_ML(){delete model;}
    void Intialize();
    Mat *Calculate_Confusion_Matrices(int type, Machine_Learning_Data_Preparation *&i_ml);
    float *Calculate_Accuracies(Mat *&i_confusion_matrix, Machine_Learning_Data_Preparation *&i_ml);
    float Calculate_Sum_Accuracy(float *&i_accuracy, Machine_Learning_Data_Preparation *&i_ml);
    void Calculate_Result();
    void Return_Parameter();
    string Head_Parameter();
    void Create_Weight(string path_save);
};


// class Write_File{
// private:
//     Parent_ML *final_ml;
//     Machine_Learning_Data_Preparation *prepared_data;
//     float mean;
//     float variance; 
//     float sta_dev; 
//     int k_fold_value; 
//     Mat *con_mat;
//     char **buffer_file;
//     string file_full_path;
//     string file_the_best_full_path;
//     string file_collection_full_path;
//     ofstream file;
//     ofstream file_the_best;
//     ofstream file_collection;
// public:
//     Write_File();
//     Write_File(Parent_ML *i_final_ml, Machine_Learning_Data_Preparation *i_prepared_data,string i_number_of_CE);
//     ~Write_File();
//     // void Main_Process(float mean, float variance,float sta_dev,int k_fold_value, Mat con_mat[],char **buffer_file);
//     void Main_Process();
//     void Write_Header(string header);
//     string Create_file_path(string file_path, string file_name, string number_of_CE);
//     bool The_File_Process();
//     bool The_File_Collection_Process(float i_mean_test, float i_sta_dev_test, float i_mean_train, float i_sta_dev_train);
//     bool The_Best_Process(float i_mean, float i_variance, float i_sta_dev, int i_k_fold_value,Mat *&i_con_mat, string type);
// };

// Write_File::Write_File(Parent_ML *i_final_ml, Machine_Learning_Data_Preparation *i_prepared_data, string i_number_of_CE){
//     final_ml=i_final_ml;
//     prepared_data=i_prepared_data;
//     // mean=final_ml->mean;
//     // variance=final_ml->variance; 
//     // sta_dev=final_ml->sta_dev;
//     // k_fold_value=prepared_data->k_fold_value;
//     // con_mat=final_ml->confusion_matrix;
//     // buffer_file=final_ml->result_buffer;
//     // cout<<"i_number_of_CE: "<<i_number_of_CE<<endl;
//     string file_path="resource/rf/";
//     // file_full_path=Create_file_path(file_path,"min_sample_count_",i_number_of_CE);
//     file_the_best_full_path=Create_file_path(file_path,"Calculate_standard_deviation_",i_number_of_CE);
//     file_collection_full_path=Create_file_path(file_path,"accuracy_collection_",i_number_of_CE);

//     // file.open(file_full_path, std::ios_base::app);
//     file_collection.open(file_collection_full_path, std::ios_base::app);
//     file_the_best.open(file_the_best_full_path, std::ios_base::app);

// }

// Write_File::~Write_File(){
//     file.close();
//     file_the_best.close();
//     file_collection.close();
// }
// void Write_File::Write_Header(string header){
//     file_collection<<header;
//     file_collection<<endl;
//     // file<<final_ml->Head_Parameter();
//     file<<endl;
// }
// void Write_File::Main_Process(){
//     // The_Best_Process();
//     The_File_Collection_Process(final_ml->mean_test, final_ml->sta_dev_test, final_ml->mean_train, final_ml->sta_dev_train);
//     // The_File_Process();

//     // cout<<"final_ml->mean_test: "<<final_ml->mean_test<<endl;
//     // cout<<"final_ml->variance_test: "<<final_ml->variance_test<<endl;
//     // cout<<"final_ml->sta_dev_test: "<<final_ml->sta_dev_test<<endl;
//     // cout<<"prepared_data->k_fold_value: "<<prepared_data->k_fold_value<<endl;
//     // cout<<"final_ml->confusion_matrix_test: "<<final_ml->confusion_matrix_test<<endl;


//     The_Best_Process(final_ml->mean_test, final_ml->variance_test, final_ml->sta_dev_test, 
//         prepared_data->k_fold_value, final_ml->confusion_matrix_test, "test");
//     The_Best_Process(final_ml->mean_train, final_ml->variance_train, final_ml->sta_dev_train, 
//         prepared_data->k_fold_value, final_ml->confusion_matrix_train, "train");

// }

// string Write_File::Create_file_path(string file_path, string file_name, string number_of_CE){
//     string file_name_extension=".txt";
//     string full_file_name=file_name+number_of_CE+file_name_extension;
//     string temp_file_full_path=file_path+full_file_name;
//     cout<<"temp_file_full_path: "<<temp_file_full_path<<endl; 
//     return temp_file_full_path;
// }

// bool Write_File::The_File_Process(){
//     for(int i=0;i<k_fold_value;i++){
//         file<<buffer_file[i];
//     }
//     return true;        
// }

// bool Write_File::The_File_Collection_Process(float i_mean_test, float i_sta_dev_test, float i_mean_train, float i_sta_dev_train){
//     // char mse_buffer_test[70];
//     // char mse_buffer_train[70];
//     // sprintf(mse_buffer_test, "%1.f ± %1.f%% \n", i_mean_test*100, i_sta_dev_test*100);
//     // sprintf(mse_buffer_train, "%1.f ± %1.f%% \n", i_mean_train*100, i_sta_dev_train*100);
//     // file_collection<<"#Mean, Variance, Sta_dev, Mean Square Error";
//     file_collection<<final_ml->result_buffer;
//     // file_collection<<endl;
//     // file_collection<<to_string(mean);
//     // file_collection<<", ";
//     // file_collection<<to_string(variance);
//     // file_collection<<", ";
//     // file_collection<<to_string(sta_dev);
//     // file_collection<<", ";
//     // file_collection<<mse_buffer;
//     // file_collection<<endl;
//     // file_collection<<mse_buffer_test;
//     // file_collection<<", ";
//     // file_collection<<to_string(i_mean_test);
//     // file_collection<<endl; 
    
//     return true;
// }

// bool Write_File::The_Best_Process(float i_mean, float i_variance, float i_sta_dev, int i_k_fold_value, Mat *&i_con_mat, string type){
//     char mean_buffer[20],variance_buffer[40],sta_dev_buffer[40],mse_buffer[70];
//     sprintf(mean_buffer, "#mean: %f \n", i_mean);
//     sprintf(variance_buffer, "#variance: %f \n", i_variance);
//     sprintf(sta_dev_buffer, "#sta_dev: %f \n", i_sta_dev);  //header
//     sprintf(mse_buffer, "#Mean Square Error: %1.f ± %1.f%% \n", i_mean*100, i_sta_dev*100);
//     cout<<"start Best"<<endl;
//     cout<<"mean:"<<mean_buffer<<endl;
//     cout<<"variance_buffer:"<<variance_buffer<<endl;
//     cout<<"sta_dev_buffer:"<<sta_dev_buffer<<endl;
//     cout<<"mse_buffer:"<<mse_buffer<<endl;
//     cout<<"end Best"<<endl;
//     file_the_best << "----------";
//     file_the_best << type;
//     file_the_best << "----------\n";
//     if (file_the_best){
//         // file_the_best<<"\n\n";    
//         file_the_best<<mean_buffer;
//         file_the_best<<variance_buffer;
//         file_the_best<<sta_dev_buffer;
//         file_the_best<<mse_buffer;
//         file_the_best<<"\n\n";
//         file_the_best<<"#Confusion Matrix\n";
//         for(int i=0; i<i_k_fold_value; i++){
//             char buffer[50];
//             sprintf(buffer, "#k=%d\n", i);  //header
//             file_the_best<<buffer;
//             file_the_best<<"#";
//             file_the_best<<i_con_mat[i].at<int>(0,0);
//             file_the_best<<", ";
//             file_the_best<<i_con_mat[i].at<int>(0,1);
//             file_the_best<<"\n";
//             file_the_best<<"#";
//             file_the_best<<i_con_mat[i].at<int>(1,0);
//             file_the_best<<", ";
//             file_the_best<<i_con_mat[i].at<int>(1,1);
//             file_the_best<<"\n\n";
//         }
            
//     }
//     file_the_best << "--------";
//     file_the_best << type;
//     file_the_best << " end";
//     file_the_best << "--------\n";
//     return 0;
// }
#endif