#ifndef SurfDetection_hpp
#define SurfDetection_hpp

#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"

// Using namespace cv and std so we dont have to write them again in the program.
using namespace cv;
using namespace std;

// FUNSTIONS IN SURF FILE
void getReferenceImage();
bool matchFeatures(Mat _frame);//, vector<KeyPoint> targetPoints, Mat targetDesc);
//void drawLinesAroundDetectedObject();
void getDefaultReferenceCoordinates();
void drawLinesAroundDetectedObject(vector<DMatch> good_matches, Mat _frame);
void getDefaultReferenceCoordinates();
bool performSurfDetection(Mat image,bool referenceOrNot);
void getRefImageAndPerformSURF();
bool SurfMain(Mat detectedFrame);

#endif /* SurfDetection_hpp */
