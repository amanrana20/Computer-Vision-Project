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

// GLOBAL VARIABLES
bool condition = true;
int thresholdValue;

const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;
const int MIN_OBJECT_AREA = 30 * 30;

bool changePermission = true;

// Hue values range
int h_min = 0;
int h_max = 255;

// saturation values range
int s_min = 0;
int s_max = 255;

// Value values range
int v_min = 0;
int v_max = 255;

Mat frame, frame_gray;
vector<vector<Point>> contours;
vector<Vec4i> hierarchy;

int ractangleBoundsConstant = 50;
// GLOBAL VARIABLES END

void createAWindow() {
    namedWindow("Threshold Image", CV_WINDOW_OPENGL);
}
void destroyTheWindow() {
    destroyWindow("Threshold Image");
}

void hueMinValueChange(int value, void* userInfo) {
    
    h_min = value;
}

void hueMaxValueChange(int value, void* userInfo) {
    
    h_max = value;
}

void saturationMinValueChange(int value, void* userInfo) {
    
    s_min = value;
}
void saturationMaxValueChange(int value, void* userInfo) {
    
    s_max = value;
}

void valueMinValueChange(int value, void* userInfo) {
    
    v_min = value;
}

void valueMaxValueChange(int value, void* userInfo) {
    
    v_max = value;
}

void loadValuesToTrackBar() {
    setTrackbarPos("Hue Min:", "Threshold Image", h_min);
    setTrackbarPos("Hue Max:", "Threshold Image", h_max);
    setTrackbarPos("Saturation Min:", "Threshold Image", s_min);
    setTrackbarPos("Saturation Max:", "Threshold Image", s_max);
    setTrackbarPos("Value Min:", "Threshold Image", v_min);
    setTrackbarPos("Value Max:", "Threshold Image", v_max);
}

void createTheTrackerbar() {
    
    createTrackbar("Hue Min:", "Threshold Image", 0, 255, hueMinValueChange);
    createTrackbar("Hue Max:", "Threshold Image", 0, 255, hueMaxValueChange);
    
    createTrackbar("Saturation Min:", "Threshold Image", 0, 255, saturationMinValueChange);
    createTrackbar("Saturation Max:", "Threshold Image", 0, 255, saturationMaxValueChange);
    
    createTrackbar("Value Min:", "Threshold Image", 0, 255, valueMinValueChange);
    createTrackbar("Value Max:", "Threshold Image", 0, 255, valueMaxValueChange);
    
}

//void set_HSV_Values(int a, int b, int c, int d, int e, int f) {
//
//    h_min = a;
//    s_min = b;
//    v_min = c;
//
//    h_max = d;
//    s_max = e;
//    v_max = f;
//
//}

void closeImg() {
    Mat strucElement = getStructuringElement(0, Size(15,15));
    dilate(frame_gray, frame_gray, strucElement);
    erode(frame_gray, frame_gray, strucElement);
}

int findLargetContourArea(vector<vector<Point>> _contours) {
    int largestArea = 0, pos = 0;
    for (int i = 0; i < _contours.size(); i++) {
        int area = contourArea(_contours[i]);
        if (area > largestArea) {
            largestArea = area;
            pos = i;
        }
    }
    return pos;
}

int main() {
    
    VideoCapture cap(0);
    
    cap.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
    
    createAWindow();
    createTheTrackerbar();
    
    for(;;) {
        
        cap >> frame;
        cvtColor(frame, frame_gray, COLOR_BGR2HSV);
        inRange( frame_gray, Scalar(h_min,s_min, v_min), Scalar(h_max, s_max, v_max), frame_gray);
        closeImg();
        closeImg();
        imshow("Thresholded Image", frame_gray);
        
        findContours(frame_gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
        
        int pos = findLargetContourArea(contours);
        
        if (contours.size() > 0) {
            if (contourArea(contours[pos]) >= MIN_OBJECT_AREA) {
                Rect data = boundingRect(contours[0]);
                rectangle(frame, Point(data.x - ractangleBoundsConstant, data.y -  ractangleBoundsConstant), Point(data.x + data.width + ractangleBoundsConstant, data.y + data.height + ractangleBoundsConstant), Scalar(0,255,0));
            }
            
        }
        
        imshow("Threshold Image", frame);
        
        int wait = waitKey(30);
        if(wait >= 0) {
            if (wait == 99) {
                //set_HSV_Values(h_min, s_min, v_min, h_max, s_max, v_max);
                cout << h_min << " " << s_min << " " << v_min << " " << h_max << " " << s_max << " " << v_max << endl;
                changePermission = !changePermission;
                
                destroyTheWindow();
                createAWindow();
                
                if (changePermission == true) {
                    createTheTrackerbar();
                    loadValuesToTrackBar();
                }
                
                
            }else {
                cout << h_min << " " << s_min << " " << v_min << " " << h_max << " " << s_max << " " << v_max << endl;
                break;
            }
        }
    }
    
    destroyAllWindows();
    
    return 0;
}
