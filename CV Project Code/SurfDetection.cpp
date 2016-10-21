#include "SurfDetection.hpp"

// global variables - SURF
Mat refImage, refDescriptors;
Mat H;
Mat img_matches;
vector<KeyPoint> refKeyPoints, targetKeyPoints;
std::vector<Point2f> ref_obj_corners(4);
Mat outputImage, targetDescriptors;
int i;

// VARIABLES TO FINETUNE THE OBJECT DETECTION
int matchThreshold = 1;
int numberOfMatchingPoints = 15;  // This is the size of 'good_matches' vector array, obtained by taking the best matches from 'matches' vector array.

void getReferenceImage() {
    
     // Setting the videoCapture object 'cap' to receive feed from the webcam i.e. (0).
     VideoCapture cap(0);
     
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
    
    refImage = imread("/Users/amanrana/Desktop/refImage.jpg");
    if (refImage.empty() == false) {
        cvtColor(refImage, refImage, CV_RGB2GRAY);
    }
    cout << "Reference image has been captured successfully !" << endl << endl;
    
}

bool performSurfDetection(Mat image,bool referenceOrNot) {
    
    bool goodMatchesDetected;
    vector<KeyPoint> keyPoints; // Vector definition to be used in the SURF algorithm.
    
    // Defining SurfFeatureDetector
    int hessian = 2000; // setting the hessian to 2500
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
        imshow("Key Points in frame1", outputImage); // TESTING - DELETE AFTER USE OVER
    } else {
        targetKeyPoints = keyPoints;
        targetDescriptors = descriptors;
        imshow("Key Points in frame", outputImage); // TESTING - DELETE AFTER USE OVER
        return goodMatchesDetected = matchFeatures(image);
    }
    return false;
}

void getRefImageAndPerformSURF() {
    // Get the reference image and store it in (Mat) refImage.
    getReferenceImage();
    performSurfDetection(refImage, true); // TRUE will tell the performSurfDetection function whether this is the reference image or not
}

bool matchFeatures(Mat _frame) {
    
    // Matching descriptor vectors with a brute force matcher
    BFMatcher matcher(NORM_L2);
    vector<DMatch> matches, good_matches;
    matcher.match(targetDescriptors, refDescriptors, matches);
    
    unsigned long int matchSize = matches.size();
    
    if (matchSize > numberOfMatchingPoints) {
        
        for (int i = 0; i < matchSize; i++) {
            
            // TESTING -> Delete if the project runs successfully 95% of the time.
            //cout << int(matches[i].queryIdx) << " " << int(matches[i].trainIdx) << " " << int(matches[i].trainIdx) << " " << int(matches[i].queryIdx) << endl;
            
            if (( abs( int(matches[i].queryIdx) - int(matches[i].trainIdx) ) <= matchThreshold) || ( abs(int(matches[i].trainIdx) - int(matches[i].queryIdx)) <= matchThreshold)) {
                
                good_matches.push_back(matches[i]);
                
                if (good_matches.size() == numberOfMatchingPoints) {
                    break;
                }
                
            }
        }
    }
    
    if (good_matches.size() > 0) {
        
        return true;
        drawMatches(refImage, refKeyPoints, _frame, targetPoints, good_matches, img_matches, Scalar(255,255,255));
        drawLinesAroundDetectedObject(good_matches, _frame);
        
    } else { cout << "good_matches.size > 0 IS FALSE. Exiting..." << endl; return false; }
}

 void drawLinesAroundDetectedObject(vector<DMatch> good_matches, Mat _frame) {
 
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

     centerX = int(centerX / i);  // averaging the X values after summation
     centerY = int(centerY / i);  // averaging the Y values after summation

     rectangle(_frame, Point2i(centerX - int(refImage.size().height / 8), centerY -
     int(refImage.size().width / 8)), Point2i(centerX + int(refImage.size().height / 6), centerY +
     int(refImage.size().width / 6)), Scalar(0,255,0), 2);

     imshow( "Good Matches & Object detection", _frame );
 
 }

bool SurfMain(Mat detectedFrame) {
    
    if (detectedFrame.cols > 0 && detectedFrame.rows > 0 ) {
        
        // Converting to grayscale
        cvtColor(detectedFrame, detectedFrame, CV_BGR2GRAY);
        
        // Resize the frame to (700,437)
        resize(detectedFrame, detectedFrame, Size(700, 437));
        
        // Perform SURF Detection on every frame
        return performSurfDetection(detectedFrame, false); // false - because the frame is not the reference Image
        
    } else { cout << "detected frame has size zero.. exiting.." << endl; return false; }
} // end of int main()
