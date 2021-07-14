#include "machine_learning_data_preparation.h"

Machine_Learning_Data_Preparation::Machine_Learning_Data_Preparation(Mat i_data, Mat i_responses, Mat i_CEs_data, vector<string> i_imgname, string* i_img_root,
    int i_ntrain_samples, int i_ntest_samples, 
    const string& i_filename_to_save, const string& i_filename_to_load, int the_number_of_class, int the_number_of_CEs ){    
    // cout<<"filename_to_save: "<<&filename_to_save<<endl;
    data=i_data;
    responses=i_responses;
    ntrain_samples=i_ntrain_samples;
    ntest_samples=i_ntest_samples;
    filename_to_save=i_filename_to_save;
    filename_to_load=i_filename_to_load;
    imgname=i_imgname;
    img_root=i_img_root;

    class_count=the_number_of_class;

    // the_number_of_data=data.cols;
    the_number_of_data=the_number_of_CEs;
    // cout<<"data: "<<data<<endl;
    // cout<<"the_number_of_data: "<<the_number_of_data<<endl;
    // getchar();
    n_total_samples=ntrain_samples+ntest_samples;
    k_fold_value=n_total_samples/ntest_samples;
    CEs_data=i_CEs_data;
    n_all_samples=data.rows;
    // cout<<"ini done"<<endl;
    // train_data = Mat::zeros( p_ntrain_samples, the_number_of_data, CV_32F );
    //      test_data = Mat::zeros( p_ntest_samples, the_number_of_data, CV_32F );


    //      train_responses = Mat::zeros( p_ntrain_samples, p_class_count, CV_32F );
    // test_responses = Mat::zeros( p_ntest_samples, p_class_count, CV_32F );
    //       train_responses_int = Mat::zeros( p_ntrain_samples, 1, CV_32F );
    // test_responses_int = Mat::zeros( p_ntest_samples, 1, CV_32F );
}

void Machine_Learning_Data_Preparation::Main_Process(int technique){
    //Split data
    value=0;
    // cout<<"CEs_data: "<<CEs_data<<endl;
    // cout<<"train_responses: "<<train_responses<<endl;
    // cout<<"p_class_count: "<<p_class_count<<endl;
    // cout<<"k_fold_value: "<<k_fold_value<<endl;
    n_total_samples=ntrain_samples+ntest_samples;
    ml_technique=technique;
    train_data=new Mat[k_fold_value];
    test_data=new Mat[k_fold_value];
    train_responses=new Mat[k_fold_value];
    test_responses=new Mat[k_fold_value];
    train_responses_int=new Mat[k_fold_value];
    test_responses_int=new Mat[k_fold_value];
    // cout<<"p2"<<endl;
    
    CEs_train_data=new Mat[k_fold_value];
    CEs_test_data=new Mat[k_fold_value];
    // cout<<"p3"<<endl;

    train_img_path=new vector<string>[k_fold_value];
    test_img_path=new vector<string>[k_fold_value];
    
    tdata=new Ptr<TrainData>[k_fold_value];

    while(1){
        // cout<<"hello"<<endl;
        if(value==k_fold_value){break;}
        train_data[value] = Mat::zeros( ntrain_samples, the_number_of_data, CV_32F );
        test_data[value] = Mat::zeros( ntest_samples, the_number_of_data, CV_32F );

        // train_data[value] = Mat::zeros( ntrain_samples, the_number_of_data, CV_32FC1 );
        // test_data[value] = Mat::zeros( ntest_samples, the_number_of_data, CV_32FC1 );

        CEs_train_data[value] = Mat::zeros( ntrain_samples, the_number_of_data, CV_32F );//20 is CEs values
        CEs_test_data[value] = Mat::zeros( ntest_samples, the_number_of_data, CV_32F );//20 is CEs values

        if(ml_technique==0){
            train_responses[value] = Mat::zeros( ntrain_samples, class_count, CV_32F );
            test_responses[value] = Mat::zeros( ntest_samples, class_count, CV_32F );
            train_responses_int[value] = Mat::zeros( ntrain_samples, 1, CV_32F );
            test_responses_int[value] = Mat::zeros( ntest_samples, 1, CV_32F );
        }else if(ml_technique==1){
            // train_responses[value] = Mat::zeros( ntrain_samples, class_count, CV_32S );
            // test_responses[value] = Mat::zeros( ntest_samples, class_count, CV_32S );
            //train_responses_int[value] = Mat::zeros( ntrain_samples, 1, CV_32S );
            // test_responses_int[value] = Mat::zeros( ntest_samples, 1, CV_32S );

            train_responses[value] = Mat::zeros( ntrain_samples, class_count, CV_32S);
            test_responses[value] = Mat::zeros( ntest_samples, class_count, CV_32S );
            train_responses_int[value] = Mat::zeros( ntrain_samples, 1, CV_32S );
            test_responses_int[value] = Mat::zeros( ntest_samples, 1, CV_32S );
        }else if(ml_technique==2){
            train_responses[value] = Mat::zeros( ntrain_samples, class_count, CV_32F );
            test_responses[value] = Mat::zeros( ntest_samples, class_count, CV_32F );
            train_responses_int[value] = Mat::zeros( ntrain_samples, 1, CV_32F );
            test_responses_int[value] = Mat::zeros( ntest_samples, 1, CV_32F );

            // train_responses[value] = Mat::zeros( ntrain_samples, class_count, CV_32FC1 );
            // test_responses[value] = Mat::zeros( ntest_samples, class_count, CV_32FC1 );
            // train_responses_int[value] = Mat::zeros( ntrain_samples, 1, CV_32FC1 );
            // test_responses_int[value] = Mat::zeros( ntest_samples, 1, CV_32FC1 );
            
        }else{
            train_responses[value] = Mat::zeros( ntrain_samples, class_count, CV_32S );
            test_responses[value] = Mat::zeros( ntest_samples, class_count, CV_32S );
            train_responses_int[value] = Mat::zeros( ntrain_samples, 1, CV_32S );
            test_responses_int[value] = Mat::zeros( ntest_samples, 1, CV_32S );
            // train_responses[value] = Mat::zeros( ntrain_samples, class_count, CV_32FC1 );
            // test_responses[value] = Mat::zeros( ntest_samples, class_count, CV_32FC1 );
            // train_responses_int[value] = Mat::zeros( ntrain_samples, 1, CV_32FC1 );
            // test_responses_int[value] = Mat::zeros( ntest_samples, 1, CV_32FC1 );

        }
        // cout<<"hello2"<<endl;
        // cout<<"p_ntest_samples: "<<p_ntest_samples<<endl;
        int block=value;
        // int block=ntest_samples*value;
        // cout<<"k_fold_value: "<<k_fold_value<<endl;
        // getchar();


        // Split_train_test_data(&train_data[value], &test_data[value], &train_responses[value], &test_responses[value],
        //             &train_responses_int[value], &test_responses_int[value], &CEs_train_data[value], &CEs_test_data[value],
        //             &train_img_path[value], &test_img_path[value],
        //             imgname, data, responses, CEs_data,
        //             block, the_number_of_data, n_total_samples, ntest_samples,ml_technique);

        Split_train_test_data2(&train_data[value], &test_data[value], &train_responses[value], &test_responses[value],
                    &train_responses_int[value], &test_responses_int[value], &CEs_train_data[value], &CEs_test_data[value],
                    &train_img_path[value], &test_img_path[value],
                    imgname, data, responses, CEs_data, k_fold_value,
                    value, the_number_of_data, n_total_samples, ntest_samples,ml_technique);

        // cout<<"block: "<<block<<endl;
        // getchar();
        if(block==11){break;}
        if(ml_technique==0){//ANN
            tdata[value] = TrainData::create(train_data[value], ROW_SAMPLE, train_responses[value]);//train_responses: 2col many rows
        }else if(ml_technique==1){//Boost
            tdata[value] = TrainData::create(train_data[value], ROW_SAMPLE, train_responses_int[value]);//train_responses_int: 1 col many rows
            // tdata[value] = TrainData::create(train_data[value], ROW_SAMPLE, train_responses[value]);//train_responses_int: 1 col many rows
        }else if(ml_technique==2){//RF
            tdata[value] = TrainData::create(train_data[value], ROW_SAMPLE, train_responses_int[value]);
        }else{
            tdata[value] = TrainData::create(train_data[value], ROW_SAMPLE, train_responses_int[value]);
        }
        // cout<<"p4"<<endl;
        // Save_Path_of_Splited_Test_with_Train(value,"Data_splited_path", img_root, train_img_path[value], test_img_path[value]);
        // Save_Path_of_Splited_Test_with_Train(value,"Data_splited_path", "/content/final_dataset", train_img_path[value], test_img_path[value]);
        


        // cout<<"train_data: "<<train_data[value]<<endl;
        // getchar();
        value++;
    }
    // cout<<"k_fold_value: "<<k_fold_value<<endl;
    // getchar();
    // tdata = TrainData::create(train_data, ROW_SAMPLE, train_responses);
    // tdata = TrainData::create(train_data, ROW_SAMPLE, train_responses_int);
        // cout<<"block: "<<block<<endl;
    // cout<<"p5"<<endl;
        // cout<<"test_responses: "<<test_responses<<endl;
        // cout<<"end"<<endl;
        // getchar();
}
void Machine_Learning_Data_Preparation::Main_Process2(string path_save ,int technique, int p1, int p2, float p3, int p4, int p5){
    // cout<<"p1"<<endl;
    // cout<<""
    ml_technique=technique;
    // cout<<"ml_technique: "<<ml_technique<<endl;
    // cout<<"the_number_of_data: "<<the_number_of_data<<endl;
     // main_data = Mat::zeros( data.rows, the_number_of_data, CV_32F );
    // main_data=data.rowRange(0, data.rows);
    main_data=data.rowRange(0, data.rows);
    
    if(ml_technique==0){
        main_responses = Mat::zeros( data.rows, class_count, CV_32F );
        main_responses_int = Mat::zeros( data.rows, 1, CV_32F );

    }else if(ml_technique==1){
        main_responses = Mat::zeros( data.rows, class_count, CV_32S);
        main_responses_int = Mat::zeros( data.rows, 1, CV_32S );
        
    }else if(ml_technique==2){
        // main_responses = Mat::zeros( data.rows, class_count, CV_32F );
        // main_responses_int = Mat::zeros( data.rows, 1, CV_32F );

        main_responses = Mat::zeros( data.rows, class_count, CV_32F );
        main_responses_int = Mat::zeros( data.rows, 1, CV_32F );
        // cout<<"in here"<<endl;
    }else{
        main_responses = Mat::zeros( data.rows, class_count, CV_32F );
        main_responses_int = Mat::zeros( data.rows, 1, CV_32F );
        // cout<<"in here"<<endl;
        // getchar();
    }
    // cout<<"p1"<<endl;
    // cout<<"data.rows:"<<data.rows<<endl;/


    // cout<<"main_responses_int: "<<main_responses_int<<endl;
    // getchar();

    for(int i=0;i<data.rows;i++){
        // cout<<"i:"<<i<<endl;
        int cls_label = responses.at<int>(i);
        // cout<<"i:"<<i<<endl;
        if(ml_technique==1){
            if(cls_label==1){
                main_responses.at<int>(i, cls_label) = 1;
                // test_responses_int->at<int>(i_test,0)=cls_label;
                main_responses_int.at<int>(i,0)=1;
            }
        }else{
            // cout<<"i:"<<i<<endl;
            // cout<<"cls_label:"<<cls_label<<endl;
            main_responses.at<float>(i, cls_label) = 1.f;
            // cout<<"i:"<<i<<endl;
            main_responses_int.at<float>(i,0)=cls_label;
            // cout<<"i:"<<i<<endl;
        }

        // for(int j=0;j<the_number_of_data;j++){
        //     if(ml_technique==1){
        //         main_data.at<int>(i,j)=data.at<int>(i,j);
        //     }else{
        //         main_data.at<float>(i,j)=data.at<float>(i,j);
        //     }
        // }
    }
    // for(int i=0;i<data.ro;

    system_clock::time_point start = system_clock::now();
    if(ml_technique==0){//ANN
        tdata_for_all= TrainData::create(main_data, ROW_SAMPLE, main_responses);

        cout<<"main_data:"<<main_data.cols<<endl;
        cout<<"main_responses:"<<main_responses.cols<<endl;
        cout<<"start initialize ANN"<<endl;
        // cout<<"ml->k_fold_value: "<<ml->k_fold_value<<endl;

        int max_iter=p1;
        float method_param=p3;

        cout<<"ml->the_number_of_data: "<<the_number_of_data<<endl;
        int layer_sz[] = { the_number_of_data, 100, 100, class_count };
        cout <<  " sizeof layer_sz " << sizeof(layer_sz) << " sizeof layer_sz[0]) " << sizeof(layer_sz[0]) << endl;
        int nlayers = (int)(sizeof(layer_sz)/sizeof(layer_sz[0]));
        cout << " nlayers  " << nlayers << endl;    
        Mat layer_sizes( 1, nlayers, CV_32S, layer_sz );
        int a_function = ANN_MLP::SIGMOID_SYM;//default


        Ptr<ANN_MLP> model_final;
        cout<<" Training the classifier (may take a few minutes)...\n"<<endl;
        model_final = ANN_MLP::create();
        model_final->setLayerSizes(layer_sizes);
        model_final->setActivationFunction(a_function, 0, 0);
        // model->setActivationFunction(ANN_MLP::IDENTITY, 0, 0);
        model_final->setTermCriteria(TC(50000,0.00001));
        // cout<<"what"<<endl;
        model_final->train(tdata_for_all);
        /*Caculate performance of program*/
        system_clock::time_point end = system_clock::now();
        double seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
        test_and_save_classifier(model_final, main_data, main_responses, data.rows, 0, path_save, ml_technique);
        
    }else if(ml_technique==1){//Boost
        tdata_for_all= TrainData::create(main_data, ROW_SAMPLE, main_responses_int);
        
        int boost_type=p1;//Gentle 0.5 and true{DISCRETE, REAL, LOGIT, GENTLE}
        int weak_count=p2;
        float weight_trim_rate=p3;
        int max_depth=p4;

        Ptr<Boost> model_final;
        cout <<" Training the classifier (may take a few minutes)...\n"<<endl;;
        model_final=Boost::create();
        model_final->setBoostType(boost_type);  //Gentle 0.5 and true{DISCRETE, REAL, LOGIT, GENTLE}
        model_final->setWeakCount(weak_count);       //the Gentle best=98;
        model_final->setWeightTrimRate(weight_trim_rate);//the Gentle best=0.83;
        model_final->setMaxDepth(max_depth);         //the Gentle best=2;
        model_final->setUseSurrogates(false);
        model_final->setPriors(Mat());
        model_final->train(tdata_for_all);
        /*Caculate performance of program*/
        system_clock::time_point end = system_clock::now();
        double seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
        test_and_save_classifier(model_final, main_data, main_responses_int, data.rows, 0, path_save, ml_technique);
        
        
    }else if(ml_technique==2){//RF
        tdata_for_all= TrainData::create(main_data, ROW_SAMPLE, main_responses_int);

        int max_depth=p1;
        int min_sample_count=p2;
        float regression_accuracy=p3;
        int max_categories=p4;
        int tc_value=p5;

        Ptr<RTrees> model_final;
        model_final = RTrees::create();
        model_final->setMaxDepth(max_depth);
        model_final->setMinSampleCount(min_sample_count);
        model_final->setRegressionAccuracy(regression_accuracy);
        model_final->setUseSurrogates(false /* true */);
        model_final->setMaxCategories(2);
        model_final->setTermCriteria(TC(200,0));
        model_final->train(tdata_for_all);
        cout << "Training the classifier (may take a few minutes)...\n";
        /*Caculate performance of program*/
        system_clock::time_point end = system_clock::now();
        double seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
        test_and_save_classifier(model_final, main_data, main_responses_int, data.rows, 0, path_save, ml_technique);


    }else{

    }


    
    // model_final->setTermCriteria(CV_TERMCRIT_ITER |    CV_TERMCRIT_EPS);
    
    // model_final->setTermCriteria(TermCriteria(TermCriteria::COUNT, 50, 0));
    
    // cout<<"done"<<endl;
    // cout<<"p3"<<endl;

}
// void Machine_Learning_Data_Preparation::Save_Path_of_Splited_Test_with_Train(int k_fold_value, string folder_path_for_saving_text, string root_path, vector<string> train_img_path, vector<string> test_img_path){
//     ofstream file_train(folder_path_for_saving_text+"/The_first_cycle_train"+to_string(k_fold_value)+".txt");
//     ofstream file_test(folder_path_for_saving_text+"/The_first_cycle_test"+to_string(k_fold_value)+".txt");


//     for(vector<string>::const_iterator i = train_img_path.begin(); i != train_img_path.end(); ++i){
//         file_train<<root_path+"/"+*i;
//         file_train<<endl;
//     }

//     for(vector<string>::const_iterator i = test_img_path.begin(); i != test_img_path.end(); ++i){
//         file_test<<root_path+"/"+*i;
//         file_test<<endl;
//     }

//     file_train.close();
//     file_test.close();
// }
bool Machine_Learning_Data_Preparation::Split_train_test_data2(Mat *train_data, Mat *test_data, Mat *train_responses, Mat *test_responses,
                    Mat *train_responses_int, Mat *test_responses_int, Mat *CEs_train_data, Mat *CEs_test_data,
                    vector<string> *train_img_path, vector<string> *test_img_path, 
                    vector<string> _imgname, Mat data, Mat responses, Mat CEs_data, int k_fold_value,
                    int value,int the_number_of_data, int n_total_samples, int ntest_samples, int technique)
{
    int i_train=0;
    int i_test=0;

    // cout<<"imgname: "<<imgname<<endl;
    // for (std::vector<string>::const_iterator i = imgname.begin(); i != imgname.end(); ++i)
    //     std::cout << *i << endl;
    // getchar();
    // cout<<"n_total_samples: "<<n_total_samples<<endl;
    // getchar();
    // cout<<"size: "<<imgname.size()<<endl;
    // getchar();
    // vector<string>::const_iterator img_name_index = imgname.begin();
    

    // cout<<"k_fold_value:"<<k_fold_value;
    // cout<<"k_fold_value:"<<k_fold_value;
    for(int i=0;i<n_total_samples;i++){
        // int cls_label = responses.at<int>(i) - 48;// - 'A'; //change to numerical classes, still they read as chars
        int cls_label = responses.at<int>(i);// - 'A'; //change to numerical classes, still they read as chars
        // cout << "labels " << cls_label << endl;
        // cout<<"i:"<<i<<endl;
        if(i%k_fold_value==value){
            // cout<<"i:"<<i<<endl;
            // cout<<"imgname[i]: "<<imgname[i]<<endl;
            test_img_path->push_back(imgname[i]);
            // test_img_path->push_back(*img_name_index);
            // img_name_index++;
            // getchar();
            if(ml_technique==1){
                if(cls_label==1){
                    test_responses->at<int>(i_test, cls_label) = 1;
                    // test_responses_int->at<int>(i_test,0)=cls_label;
                    test_responses_int->at<int>(i_test,0)=1;
                }
            }else{
                test_responses->at<float>(i_test, cls_label) = 1.f;
                test_responses_int->at<float>(i_test,0)=cls_label;
            }
            
        }else{//test part
            // img_name_index++;
            // cout<<"i:"<<i<<endl;
            // cout<<"imgname[i]: "<<imgname[i]<<endl;
            train_img_path->push_back(imgname[i]);
            if(ml_technique==1){

                // getchar();
                if(cls_label==1){
                    train_responses->at<int>(i_train, cls_label) = 1;
                    // train_responses_int->at<int>(i_train,0)=cls_label; 
                    train_responses_int->at<int>(i_train,0)=1; 
                }
            }else{
                train_responses->at<float>(i_train, cls_label) = 1.f;
                train_responses_int->at<float>(i_train,0)=cls_label; 
            }
            
        }
        for(int j=0;j<the_number_of_data;j++){
            if(i%k_fold_value==value){
                // cout<<"imgname[j]: "<<imgname[j]<<endl;
                // test_img_path->push_back(imgname[j]);
                if(ml_technique==1){
                    test_data->at<int>(i_test,j)=data.at<int>(i,j);
                    CEs_test_data->at<int>(i_test,j)=CEs_data.at<int>(i,j);
                }else{
                    test_data->at<float>(i_test,j)=data.at<float>(i,j);
                    CEs_test_data->at<float>(i_test,j)=CEs_data.at<float>(i,j);
                }
            }else{
                // if(value==0){
                //     if(i%k_fold_value==1){//validation data

                //     }else{
                        
                //     }
                // }else{
                //     if(i%k_fold_value==0){

                //     }else{

                //     }
                // }
                // cout<<"imgname[j]: "<<imgname[j]<<endl;
                // train_img_path->push_back(imgname[j]);
                if(ml_technique==1){
                    train_data->at<int>(i_train,j)=data.at<int>(i,j);
                    CEs_train_data->at<int>(i_train,j)=CEs_data.at<int>(i,j);
                }else{
                    train_data->at<float>(i_train,j)=data.at<float>(i,j);
                    CEs_train_data->at<float>(i_train,j)=CEs_data.at<float>(i,j); 
                }
                
            }
        }
        if( (i%k_fold_value==value) ){
            i_test++;
        }else{
            i_train++;
                
        }
    }
 //    cout<<"n_total_samples: "<<n_total_samples<<endl;
 //    cout<<"end"<<endl;
 //    // getchar();

    // cout<<"i_train: "<<i_train<<endl;
    // cout<<"i_test: "<<i_test<<endl;
    // getchar();
    return true;
}
bool Machine_Learning_Data_Preparation::Split_train_test_data(Mat *train_data, Mat *test_data, Mat *train_responses, Mat *test_responses,
                    Mat *train_responses_int, Mat *test_responses_int, Mat *CEs_train_data, Mat *CEs_test_data,
                    vector<string> *train_img_path, vector<string> *test_img_path, 
                    vector<string> _imgname, Mat data, Mat responses, Mat CEs_data,
                    int block,int the_number_of_data, int n_total_samples, int ntest_samples, int technique)
{
    int i_train=0;
    int i_test=0;

    // cout<<"imgname: "<<imgname<<endl;
    // for (std::vector<string>::const_iterator i = imgname.begin(); i != imgname.end(); ++i)
    //     std::cout << *i << endl;
    // getchar();
    // cout<<"n_total_samples: "<<n_total_samples<<endl;
    // getchar();
    // cout<<"size: "<<imgname.size()<<endl;
    // getchar();
    // vector<string>::const_iterator img_name_index = imgname.begin();
    for(int i=0;i<n_total_samples;i++){
        // int cls_label = responses.at<int>(i) - 48;// - 'A'; //change to numerical classes, still they read as chars
        int cls_label = responses.at<int>(i);// - 'A'; //change to numerical classes, still they read as chars
        // cout << "labels " << cls_label << endl;
        // cout<<"i:"<<i<<endl;
        if( (i>=block)&&(i<block+ntest_samples) ){
            // cout<<"i:"<<i<<endl;
            // cout<<"imgname[i]: "<<imgname[i]<<endl;
            test_img_path->push_back(imgname[i]);
            // test_img_path->push_back(*img_name_index);
            // img_name_index++;
            // getchar();
            if(ml_technique==1){
                if(cls_label==1){
                    test_responses->at<int>(i_test, cls_label) = 1;
                    // test_responses_int->at<int>(i_test,0)=cls_label;
                    test_responses_int->at<int>(i_test,0)=1;
                }
            }else{
                test_responses->at<float>(i_test, cls_label) = 1.f;
                test_responses_int->at<float>(i_test,0)=cls_label;
            }
            
        }else{//test part
            // img_name_index++;
            // cout<<"i:"<<i<<endl;
            // cout<<"imgname[i]: "<<imgname[i]<<endl;
            train_img_path->push_back(imgname[i]);
            if(ml_technique==1){

                // getchar();
                if(cls_label==1){
                    train_responses->at<int>(i_train, cls_label) = 1;
                    // train_responses_int->at<int>(i_train,0)=cls_label; 
                    train_responses_int->at<int>(i_train,0)=1; 
                }
            }else{
                train_responses->at<float>(i_train, cls_label) = 1.f;
                train_responses_int->at<float>(i_train,0)=cls_label; 
            }
            
        }
        for(int j=0;j<the_number_of_data;j++){
            if( (i>=block)&&(i<block+ntest_samples) ){
                // cout<<"imgname[j]: "<<imgname[j]<<endl;
                // test_img_path->push_back(imgname[j]);
                if(ml_technique==1){
                    test_data->at<int>(i_test,j)=data.at<int>(i,j);
                    CEs_test_data->at<int>(i_test,j)=CEs_data.at<int>(i,j);
                }else{
                    test_data->at<float>(i_test,j)=data.at<float>(i,j);
                    CEs_test_data->at<float>(i_test,j)=CEs_data.at<float>(i,j);
                }
            }else{
                // cout<<"imgname[j]: "<<imgname[j]<<endl;
                // train_img_path->push_back(imgname[j]);
                if(ml_technique==1){
                    train_data->at<int>(i_train,j)=data.at<int>(i,j);
                    CEs_train_data->at<int>(i_train,j)=CEs_data.at<int>(i,j);
                }else{
                    train_data->at<float>(i_train,j)=data.at<float>(i,j);
                    CEs_train_data->at<float>(i_train,j)=CEs_data.at<float>(i,j); 
                }
                
            }
        }
        if( (i>=block)&&(i<block+ntest_samples) ){
            i_test++;
        }else{
            i_train++;
                
        }
    }
 //    cout<<"n_total_samples: "<<n_total_samples<<endl;
 //    cout<<"end"<<endl;
 //    // getchar();

 //    cout<<"i_train: "<<i_train<<endl;
    // cout<<"i_test: "<<i_test<<endl;
    // getchar();
    return true;
}
Mat Machine_Learning_Data_Preparation::test_and_save_classifier(const Ptr<StatModel>& model,
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

void Machine_Learning_Data_Preparation::Save_Path_of_Splited_Test_with_Train(int k_fold_value, string folder_path_for_saving_text, string root_path, vector<string> train_img_path, vector<string> test_img_path){
    ofstream file_train(folder_path_for_saving_text+"/The_first_cycle_train"+to_string(k_fold_value)+".txt");
    ofstream file_test(folder_path_for_saving_text+"/The_first_cycle_test"+to_string(k_fold_value)+".txt");
    ofstream file_val(folder_path_for_saving_text+"/The_first_cycle_val"+to_string(k_fold_value)+".txt");


    for(vector<string>::const_iterator i = train_img_path.begin(); i != train_img_path.end(); ++i){
        file_train<<root_path+"/"+*i;
        file_train<<endl;
    }
    int index=0;
    for(vector<string>::const_iterator i = test_img_path.begin(); i != test_img_path.end(); ++i){
        // cout<<"test_img_path Size: "<<test_img_path.size()<<endl;
        if( index%2==0 ){
            file_test<<root_path+"/"+*i;
            file_test<<endl;
        }else{
            file_val<<root_path+"/"+*i;
            file_val<<endl;
        }
        index++;
    }

    file_train.close();
    file_test.close();
    file_val.close();
}