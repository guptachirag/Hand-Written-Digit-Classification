#include "commonfile.h"
#include <vector>
#include <iostream>
#include <opencv2/ml/ml.hpp>
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

//command to run g++ -I"C:\opencv\build\include" -L"C:\opencv\build\x86\mingw\lib" main.cpp feature_extraction.cpp -lopencv_core242 -lopencv_ml242 -o main

int n_trimg = 1000;   
int n_testimg = 1000;
int n_features = 214;

int main()
{
    char tr_imagedb[]="train-images.idx3-ubyte";
    char tr_labledb[]="train-labels.idx1-ubyte";
    char test_imagedb[]="t10k-images.idx3-ubyte";
    char test_labledb[]="t10k-labels.idx1-ubyte";
	
	vector < vector<float> > tr_data(n_trimg,vector<float>(n_features));
	vector <int> tr_lables(n_trimg);
	vector < vector<float> > test_data(n_testimg,vector<float>(n_features));
	vector <int> test_lables(n_testimg);
	
	// Reading Databases
	cout<<"Reading Databases...\n";
    tr_data=read_images_extract_features(tr_imagedb,n_trimg);
	cout<<"fine traing data...\n";
    tr_lables=read_lables(tr_labledb,n_trimg);
	cout<<"fine traing labels...\n";
    test_data=read_images_extract_features(test_imagedb,n_testimg);
	cout<<"fine testing data...\n";
	test_lables=read_lables(test_labledb,n_testimg);
	cout<<"fine testing labels...\n";
	
/**------------------------------------------------------------------------------SVM---------------------------------------------------------------------------------------**/	
	
	cout<<"Running SVM...\n";
	//Putting all data to Matrix of opencv
	Mat tr_dataMat(n_trimg, n_features, CV_32FC1);  
	Mat tr_lablesMat(n_trimg, 1, CV_32SC1);
	Mat test_dataMat(n_testimg, n_features, CV_32FC1);  
	Mat test_lablesMat(n_testimg, 1, CV_32SC1);
	Mat tr_results(n_trimg, 1, CV_32SC1);
	Mat test_results(n_testimg, 1, CV_32SC1);
	
	for(int i=0;i<n_trimg;++i)
	{
		for(int j=0;j<n_features;++j)
		{
			tr_dataMat.at<float>(i,j)=tr_data[i][j];
		}
	}
	
	for(int i=0;i<n_trimg;++i)
	{
		tr_lablesMat.at<int>(i,0)=tr_lables[i];
	}
	
	for(int i=0;i<n_testimg;++i)
	{
		for(int j=0;j<n_features;++j)
		{
			test_dataMat.at<float>(i,j)=test_data[i][j];
		}
	}
	for(int i=0;i<n_testimg;++i)
	{
		test_lablesMat.at<int>(i,0)=test_lables[i];
	}

	//cout<<tr_dataMat<<"\n\n\n";
	//cout<<tr_lablesMat<<"\n\n\n";
	//cout<<test_dataMat<<"\n\n\n";
	//cout<<test_lablesMat<<"\n\n\n";
	
	// parameters for SVM
	CvSVMParams params;
    params.svm_type    = CvSVM::NU_SVC;
    params.kernel_type = CvSVM::POLY;
    params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);
	
	params.degree        = CvSVM::POLY;
	params.gamma         = CvSVM::POLY;
	params.coef0         = CvSVM::POLY;
	params.nu            = 0.1;
	params.p             = CvSVM::EPS_SVR;
	
	CvSVM SVM;
    SVM.train(tr_dataMat, tr_lablesMat, Mat(), Mat(), params); 

	SVM.predict(tr_dataMat, tr_results);
	SVM.predict(test_dataMat, test_results);
	
	//cout<<tr_results<<"\n\n\n";
	//cout<<test_results<<"\n\n\n
	return 0;
}
