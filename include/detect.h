#ifndef DETECT
#define DETECT
#include "main.h"
#include <algorithm>
class P_ML{
public:
	const Ptr<StatModel> model;
	P_ML();
	~P_ML();
    int virtual Predict(const Mat& data)=0;
    // virtual void Load_Model()=0;

};

template <class T>
T *Load_ML(){
    return new T();
}

template <class T>
T *Load_ML_With_Weight(const string path_weight){
    return new T(path_weight);
}

template<class T>//base template before specialized
class C_ML : public P_ML{
public:
	void Intialize(){cout<<"error choose different technique"<<endl;}
	// void Load_Model(){cout<<"error choose different technique"<<endl;};
};

template<>//base template before specialized
class C_ML<ANN_MLP> : public P_ML{
public:
	Ptr<ANN_MLP> model;
	C_ML(){}
	~C_ML(){}
	C_ML(const string path_weight){
		model=ANN_MLP::load(path_weight);
	}
	int Predict(const Mat& data);
	// void Intialize(const Ptr<StatModel>& model, const string path_weight);
	// void Load_Model();
};

template<>//base template before specialized
class C_ML<Boost> : public P_ML{
public:
	Ptr<Boost> model;
	C_ML(){}
	~C_ML(){}
	C_ML(const string path_weight){
		model=Boost::load(path_weight);
	}
	int Predict(const Mat& data);
	// void Intialize(const Ptr<StatModel>& model, const string path_weight);
	// void Load_Model();
};

template<>//base template before specialized
class C_ML<RTrees> : public P_ML{
public:
	Ptr<RTrees> model;
	C_ML(){}
	~C_ML(){}
	C_ML(const string path_weight){
		model=RTrees::load(path_weight);
	}
	int Predict(const Mat& data);
	// void Intialize(const Ptr<StatModel>& model, const string path_weight);
	// void Load_Model();
};

template<>//base template before specialized
class C_ML<SVM> : public P_ML{
public:
	Ptr<SVM> model;
	C_ML(){}
	~C_ML(){}
	C_ML(const string path_weight){
		model=Algorithm::load<SVM>(path_weight);
	}
	int Predict(const Mat& data);
	// void Intialize(const Ptr<StatModel>& model, const string path_weight);
	// void Load_Model();
};

void Main_Detect(const string path_weight, const string path_tagfile, int ml_technique, const string source_video);
int thresholding_image(Mat image, int value, bool inverted, int window_size);
#endif