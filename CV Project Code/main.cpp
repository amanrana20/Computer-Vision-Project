#include <iostream>
#include "TrackerBar.hpp"
#include "Misc.hpp"
#include "SurfDetection.hpp"

using namespace cv;
using namespace std;

const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;
const int MIN_OBJECT_AREA = 30 * 30;
int ractangleBoundsConstant = 0;

// Hue values range
int h_min = 0;
int h_max = 180;

// saturation values range
int s_min = 0;
int s_max = 255;

// Value values range
int v_min = 0;
int v_max = 255;

vector<vector<Point>> contours;
vector<Vec4i> hierarchy;
Mat frame, frame_gray;

bool changePermission = true; // this allows the user to change the HSV-min and HSV-max values using trackerBar.

// GLOBAL VARIABLES END

int main() {
    
    getRefImageAndPerformSURF();
    
    VideoCapture cap(0);
    
    cap.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
    
    createAWindow();
    createTheTrackerbar();
    
    for(;;) {
        
        cap >> frame;
        cvtColor(frame, frame_gray, COLOR_BGR2HSV);
        // Getting the thresholded image between the HSV-min values and HSV-max values.
        inRange( frame_gray, Scalar(h_min, s_min, v_min), Scalar(h_max, s_max, v_max), frame_gray);
        // dilates and closes the thresholded image 3 times.
        closeImg();
        closeImg();
        closeImg();
        
        //if (changePermission == true) {
        imshow("B/W Image", frame_gray);
        //}
        
        // Runs only if the program is not in configeration mode
        if (changePermission != true) {
            
            // Finds the contours in each frame taken from the webcam
            findContours(frame_gray, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
            
            // Sorting the contour array in descending order of area (i.e. Largest first, smallest last)
            sort(contours.begin(), contours.end(), compareContourAreas);
            
            unsigned long int contourSize = contours.size();
            
            if (contourSize > 0) {
                
                //cout << (int)contourSize << endl;
                
                for (int i= 0; i < contourSize; i++) {
                    
                    if (contourArea(contours[i]) >= MIN_OBJECT_AREA) {
                        
                        // this functions automatically creates a rectangle bounding the contour and then we can extract the top-left point and the bottom-right point.
                        Rect boundingRectData = boundingRect(contours[i]);
                        
                        // Passing the subFrame to perform SURF detection
                        Mat temp = frame(Rect(boundingRectData.tl().x, boundingRectData.tl().y, boundingRectData.br().x - boundingRectData.tl().x, boundingRectData.br().y - boundingRectData.tl().y));
                        
                        // Passing the subFrame(-> temp) to perform SURF detection and if it returns true, then draw the rectangle around the object.
                        if (SurfMain(temp) == true) {
                            
                            putText(frame, "OBJECT DETECTED ! (in green)", Point(10,50), 1, 3.0, Scalar(0,255,0));
                            rectangle(frame, Point(boundingRectData.x - ractangleBoundsConstant, boundingRectData.y -  ractangleBoundsConstant), Point(boundingRectData.x + boundingRectData.width + ractangleBoundsConstant, boundingRectData.y + boundingRectData.height + ractangleBoundsConstant), Scalar(0,255,0));
                            
                        } // end of if (SurfMain(temp, frame) == true)
                        
                        else {
                            
                            rectangle(frame, Point(boundingRectData.x - ractangleBoundsConstant, boundingRectData.y -  ractangleBoundsConstant), Point(boundingRectData.x + boundingRectData.width + ractangleBoundsConstant, boundingRectData.y + boundingRectData.height + ractangleBoundsConstant), Scalar(255,0,0));
                            // this text is written for debugging purposes. This gets printed in the console.
                            cout << "SURF Returned false. -> Object not in frame -> OR -> wrong object passed to SURF function.." << endl;
                            
                            // This gets printed to the frame
                            putText(frame, "Potential Objects in BLUE.", Point(10, frame.rows - 50), 1, 3.0, Scalar(255,0,0));
                            
                        } // end of else
                        
                    } // end of if (contourArea(contours[0]) >= MIN_OBJECT_AREA)
                    
                } // end of for (int i= 0; i < contourSize; i++)
                
                //imshow("Threshold Image", frame);
                
            } // end of if (contourSize > 0)
            
            imshow("Threshold Image", frame);
            
        } // and of if (changePermission != true)
        
        imshow("Threshold Image", frame);
        
        // CONFIGURATION OPTIONS
        int wait = waitKey(30);
        if(wait >= 0) {
            
            if (wait == 99) {
                
                cout << h_min << " " << s_min << " " << v_min << " " << h_max << " " << s_max << " " << v_max << endl;
                changePermission = !changePermission;
                
                destroyTheWindow();
                destroyWindow("Thresholded Image");
                createAWindow();
                
                if (changePermission == false) {
                    cap.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH * 2);
                    cap.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT * 1.5);
                }
                
                if (changePermission == true) {
                    createTheTrackerbar();
                    loadValuesToTrackBar();
                    cap.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
                    cap.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
                    
                } // end of if (changePermission == true)
                
                
            } // end of if (wait == 99)
            else {
                cout << h_min << " " << s_min << " " << v_min << " " << h_max << " " << s_max << " " << v_max << endl;
                break;
            } // end of else
            
            // CONFIGURATION OPTIONS  END
            
        } // end of if(wait >= 0)
    } // end of for(;;)
    
    destroyAllWindows();
    
    return 0;
}