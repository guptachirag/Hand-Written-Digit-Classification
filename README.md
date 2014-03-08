Hand-Written-Digit-Classification
=================================

OpenCV(SVM) + C++

It contains database files from http://yann.lecun.com/exdb/mnist/

1)train-images.idx3-ubyte---- It contains 60,000 images each of size 28*28 (Used for training)
2)train-labels.idx1-ubyte---- It contains labels of above 60,000 images
3)t10k-images.idx3-ubyte----- It contains 10,000 images each of size 28*28 (Used for testing)
4)t10k-labels.idx1-ubyte----- It contains 60,000 images each of size 28*28 


It also contains two c++ source files and one c++ header file.

1)feature_extraction.cpp
It contains functions to read each image from the above databases and calculate features for them
and to read labels from the database.

2)main.cpp
It first calls the functions from the feature_extraction.cpp to read and calculate features for all 
the images and read labels.Then it uses SVM from OpenCV to classify all the training images.

The header file commonfile.h contains some of the function definition and some global variables.

Accuracy on test images - 85.4%



Kaggle\Scikit-Learn  + Python

It contains four files.

1)hwdr.py
This file contains functions to read data from .csv files downloaded from 
http://www.kaggle.com/c/digit-recognizer/data  and extract features for all training and testing images
and read labels.It then use SVM from scikit library download from http://scikit-learn.org/

2)Ptrfeatures.csv
This file contains all the features for training images.

3)Ptestfeatures.csv
This file contains all the features for test images.

4)Poutput.csv
This file contains all output labels for test images.

Accuracy on test images - 90.343%
