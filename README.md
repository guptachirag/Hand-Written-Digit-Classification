Hand-Written-Digit-Classification
=================================

#Abstract

The objective of the project is to study the accuracy in recognizing the hand written digits of MNIST database with statistical features using Support Vector Machine,Random Forrest,Extra-Trees Forrest machine learning algorithms. 

The project contains two parts :

##1. OpenCV(SVM) + C++

It works on image database files from http://yann.lecun.com/exdb/mnist/

** Language of Implementation - C++ **
** Accuracy on test images using SVM from OpenCV - 85.4% **



##2.Kaggle\Scikit-Learn + Python

It works on image database files from http://www.kaggle.com/c/digit-recognizer/data

** Language of Implementation - Python **
** Accuracy on test images using SVM from scikit - 90.343% **
** Accuracy on test images using Random Forrest from scikit - 96.971% **
** Accuracy on test images using Extra Tree Forrest from scikit - 97.071% **


##The features used in both parts are :

###first dark pixel in each row from left and right (28+28)
###first dark pixel in each col from top and bottom (28+28)
###first dark pixel in each row from center towards left and right (28+28)
###first dark pixel in each col from center towards top and bottom (28+28)
###number of dark pixels in each row and col in both halfs saperatly (28+28+28+28)
###total features = 336




