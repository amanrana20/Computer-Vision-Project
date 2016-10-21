#ifndef TrackerBar_hpp
#define TrackerBar_hpp

#include <stdio.h>
#include <stdio.h>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

void hueMinValueChange(int value, void* userInfo);
void hueMaxValueChange(int value, void* userInfo);
void saturationMinValueChange(int value, void* userInfo);
void saturationMaxValueChange(int value, void* userInfo);
void valueMinValueChange(int value, void* userInfo);
void valueMaxValueChange(int value, void* userInfo);
void loadValuesToTrackBar();
void createTheTrackerbar();



#endif /* TrackerBar_hpp */
