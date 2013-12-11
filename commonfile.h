#include<vector>

//
extern int n_trimg;     
extern int n_testimg;
extern int n_features;

//Function Defination
std::vector< std::vector<float> > read_images_extract_features(char filename[],int n_images);
std::vector<int> read_lables(char filename[],int n_images);
