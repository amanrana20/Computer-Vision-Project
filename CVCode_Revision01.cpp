/*
 
 This program is intended to implement SURF feature detector.
 We start by capturing a reference image containing the desired object to be captured.
 This is followed by starting the video feed again to capture frames from the webcam and use SURF algorithm to detect the desired object in the viddeo feed.
 
 Created by: Team Jaguar
 
 Team members:
 1. Aman Rana
 2. Syamprasad K. Rajagopalan
 3. Rohan Kothari
 4.Ulkesh Solanki
 
 */

// Including all the libraries into the code.
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

// global variables
Mat refImage, refDescriptors;
vector<KeyPoint> refKeyPoints, targetKeyPoints;
std::vector<Point2f> ref_obj_corners(4);
Mat outputImage, targetDescriptors;
Mat H, frame;
Mat img_matches;
int i;

// VARIABLES TO FINETUNE THE OBJECT DETECTION
int matchThreshold = 0;
int numberOfMatchingPoints = 5;  // This is the size of 'good_matches' vector array, obtained by taking the best matches from 'matches' vector array.



void getReferenceImage() {
    
    // Setting the videoCapture object 'cap' to receive feed from the webcam i.e. (0).
    VideoCapture cap(0);
    
    //    namedWindow("Display", CV_WND_PROP_OPENGL);  // Creates a named Window named 'Dispaly'
    //    resizeWindow("Display", 700, 437);           // Resizes the namedWindow
    
    bool condition = true;     // This condition causes the loop to run continuously till it is false
    
    // Capturing the Reference image from the webcam
    while (condition == true) {
        
        Mat boxImage1;
        cap >> boxImage1;
        imshow("Select an object to detect", boxImage1);
        cvtColor(boxImage1, boxImage1, CV_BGR2GRAY);
        
        
        if (waitKey(30) >= 0) {
            
            refImage = boxImage1;
            resize(refImage, refImage, Size(700, 437));
            condition = false;
        } // end if (waitKey(30) >= 0)
        
    } // end while(condition == true)
    
    //    // Displaying the captured image for 3000 milli seconds or 3 seconds
    //    namedWindow("Captured Image",1);
    //    imshow("BoxImage", refImage);
    cout << "Reference image has been captured successfully !" << endl << endl;
    //    waitKey(3000);
    
}



void matchFeatures(Mat frame, vector<KeyPoint> targetPoints, Mat targetDesc);
void drawLinesAroundDetectedObject();
void getDefaultReferenceCoordinates();
void drawLinesAroundDetectedObject(vector<DMatch> good_matches);
void getDefaultReferenceCoordinates();



void performSurfDetection(Mat image,bool referenceOrNot) {
    
    vector<KeyPoint> keyPoints; // Vector definition to be used in the SURF algorithm.
    
    // Defining SurfFeatureDetector
    int hessian = 2500; // setting the hessian to 2000
    SurfFeatureDetector surf(hessian);
    
    // Detecting the keypoints and storing them in keyPoints array
    surf.detect(image, keyPoints);
    
    // drawing the keypoints to outputImage using the image and keyPoints
    drawKeypoints(image, keyPoints, outputImage, Scalar(255,255,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    
    // Extracting Descriptors from images
    SurfDescriptorExtractor surfDesc;
    Mat descriptors;
    surfDesc.compute(image, keyPoints, descriptors);
    
    if (referenceOrNot == true) {
        refKeyPoints = keyPoints;
        refDescriptors = descriptors;
    } else {
        targetKeyPoints = keyPoints;
        targetDescriptors = descriptors;
        matchFeatures(image, targetKeyPoints, targetDescriptors);
    }
    
}



void matchFeatures(Mat frame, vector<KeyPoint> targetPoints, Mat targetDesc) {
    
    // Matching descriptor vectors with a brute force matcher
    BFMatcher matcher(NORM_L2);
    vector<DMatch> matches,good_matches;
    matcher.match(refDescriptors, targetDesc, matches);
    
    
    if (matches.size() > 20) {
        
        for (int i = 0; i < matches.size(); i++) {
            
            //cout << (int)matches[i].queryIdx << " " << (int)matches[i].trainIdx << endl;
            if (((int) matches[i].queryIdx - (int) matches[i].trainIdx <= matchThreshold) || ((int) matches[i].trainIdx - (int) matches[i].queryIdx <= matchThreshold)) {
                
                good_matches.push_back(matches[i]);
                
                
                if (good_matches.size() == numberOfMatchingPoints) {
                    break;
                }
                
            }
        }
    }
    
    if (matches.size() != 0) {
        
        drawMatches(refImage, refKeyPoints, frame, targetPoints, good_matches, img_matches, Scalar(255,255,255));
        namedWindow("Detected Object", WINDOW_NORMAL);
        //imshow("Detected Object", img_matches);
        
    }else {
        
        cout << "Object not detected. Exiting..." << endl;
        waitKey(1000);
        
    }
    
    
    
    drawLinesAroundDetectedObject(good_matches);
}



void drawLinesAroundDetectedObject(vector<DMatch> good_matches) {
    
    std::vector<Point2f> obj;
    std::vector<Point2f> scene;
    
    for( int i = 0; i < good_matches.size(); i++ ) {
        //-- Get the keypoints from the good matches
        obj.push_back( refKeyPoints[ good_matches[i].queryIdx ].pt );
        scene.push_back( targetKeyPoints[ good_matches[i].trainIdx ].pt );
    }
    
    int centerX = 0, centerY = 0;
    
    for (i = 0; i < good_matches.size(); i++) {
        
        centerX = (int)targetKeyPoints[ good_matches[i].trainIdx ].pt.x + centerX;
        centerY = (int)targetKeyPoints[ good_matches[i].trainIdx ].pt.y + centerY;
        
    }
    
    centerX = centerX / i;  // averaging the X values after summation
    centerY = centerY / i;  // averaging the Y values after summation
    
    //circle(frame, Point2d(centerX, centerY), 100, Scalar(0,255,0));
    rectangle(frame, Point2i((centerX - refImage.size().height / 8), centerY -
                             (refImage.size().width / 8)), Point2i((centerX + refImage.size().height / 6), (centerY +
                                                                                                            refImage.size().width / 6)), Scalar(0,255,0), 2);
    
    imshow( "Good Matches & Object detection", frame );
    
}



int main() {
    
    
    // Get the reference image and store it in Mat refImage.
    getReferenceImage();
    performSurfDetection(refImage,true);
    
    
    // Start the video capture again using the webcam.
    VideoCapture cap(0);
    
    namedWindow("Display1", CV_WINDOW_NORMAL );  // Creates a named Window named 'Dispaly'
    bool condition = true;     // This condition causes the loop to run continuously till it is false
    
    Mat frame1;
    
    while(condition == true) {
        
        // Capturing each frame from the webcam and storing it in frame matrix element.
        cap >> frame;
        cvtColor(frame, frame1, CV_BGR2GRAY);
        
        // Resize the frame to (700,437)
        resize(frame, frame, Size(700, 437));
        resize(frame1, frame1, Size(700, 437));
        
        // Perform SURF Detection on every frame
        performSurfDetection(frame, false); // false - because the frame is not reference Image
        
        
        // Wait for the user to press any key and then exit.
        if  (waitKey(30) >= 0)
            condition = false;
        
    } // end of while(condition == true)
    
    return 0;
} // end of int main()