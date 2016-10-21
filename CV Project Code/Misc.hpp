#ifndef Misc_hpp
#define Misc_hpp

#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"

using namespace cv;
using namespace std;

extern Mat frame_gray;

void closeImg();
//int* findLargeContourAreas(vector<vector<Point>> _contours);
bool compareContourAreas(vector<Point> contour1, vector<Point> contour2);
void createAWindow();
void destroyTheWindow();

#endif /* Misc_hpp */
