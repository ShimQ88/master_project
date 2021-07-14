//this class only using for create_weight class.

#ifndef MACHINE_LEARNING_DATA_PREPARATION
#define MACHINE_LEARNING_DATA_PREPARATION

#include "main.h"
// #include "ml_class.h"

class Machine_Learning_Data_Preparation{
public:

    Mat data;
    Mat responses;
    Mat CEs_data;
    string* img_root;

    vector<string> imgname;
    int ntrain_samples;
    int ntest_samples;
    int n_all_samples;
    int class_count;
    Ptr<TrainData> *tdata;
    Ptr<TrainData> tdata_for_all;
    string filename_to_save;
    string filename_to_load;

    int k_fold_value;
    int n_total_samples;
    int the_number_of_data;
    int block;
    int value;
    int ml_technique;
    // Ptr<TrainData> tdata;

    // int t(){
    //  return hello;
    // }
    Mat *train_data;
    Mat *test_data;
    
    Mat *CEs_train_data;
    Mat *CEs_test_data;

    Mat main_data;

    vector<string> *train_img_path;
    vector<string> *test_img_path;

    Mat *train_responses;
    Mat *test_responses;
    Mat *train_responses_int;
    Mat *test_responses_int;
    
    Mat main_responses;
    Mat main_responses_int;

    //////only for create weight/////
    

    Machine_Learning_Data_Preparation(){cout<<"Success Intialize"<<endl;}
    ~Machine_Learning_Data_Preparation(){
        delete tdata[k_fold_value];
        delete train_data;
        delete test_data;
        delete train_responses;
        delete test_responses;
        delete train_responses_int;
        delete test_responses_int;

        delete CEs_train_data;
        delete CEs_test_data;

        delete train_img_path;
        delete test_img_path;
    }

    Machine_Learning_Data_Preparation(Mat i_data, Mat i_responses, Mat i_CEs_data, vector<string> i_imgname, string* i_img_root, 
        int i_ntrain_samples, int i_ntest_samples, 
        const string& i_filename_to_save, const string& i_filename_to_load, int the_number_of_class, int the_number_of_CEs);

    // void Split(int input_value,int technique){
    void Main_Process(int technique);
    void Main_Process2(string path_save, int technique, int p1, int p2, float p3, int p4, int p5);
    bool Split_train_test_data(Mat *train_data, Mat *test_data, Mat *train_responses, Mat *test_responses,
                    Mat *train_responses_int, Mat *test_responses_int, Mat *CEs_train_data, Mat *CEs_test_data,
                    vector<string> *train_img_path, vector<string> *test_img_path, 
                    vector<string> _imgname, Mat data, Mat responses, Mat CEs_data,
                    int block,int the_number_of_data, int n_total_samples, int ntest_samples, int technique);
    bool Split_train_test_data2(Mat *train_data, Mat *test_data, Mat *train_responses, Mat *test_responses,
                    Mat *train_responses_int, Mat *test_responses_int, Mat *CEs_train_data, Mat *CEs_test_data,
                    vector<string> *train_img_path, vector<string> *test_img_path, 
                    vector<string> _imgname, Mat data, Mat responses, Mat CEs_data, int k_fold_value,
                    int value,int the_number_of_data, int n_total_samples, int ntest_samples, int technique);
    void Save_Path_of_Splited_Test_with_Train(int k_fold_value, string folder_path_for_saving_text, string root_path, vector<string> train_img_path, vector<string> test_img_path);
    Mat test_and_save_classifier(const Ptr<StatModel>& model,
                                     const Mat& data, const Mat& responses,
                                     int ntrain_samples, int rdelta,
                                     const string& filename_to_save, int ml_technique );
    inline TermCriteria TC(int iters, double eps);
    
};
inline TermCriteria Machine_Learning_Data_Preparation::TC(int iters, double eps){
    return TermCriteria(TermCriteria::MAX_ITER + (eps > 0 ? TermCriteria::EPS : 0), iters, eps);
}


#endif