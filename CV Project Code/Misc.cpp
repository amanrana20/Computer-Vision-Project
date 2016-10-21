#include "Misc.hpp"

void closeImg() {
    Mat strucElement = getStructuringElement(0, Size(10,10));
    dilate(frame_gray, frame_gray, strucElement);
    erode(frame_gray, frame_gray, strucElement);
}

bool compareContourAreas(vector<Point> contour1, vector<Point> contour2) {
    double i = fabs( contourArea(cv::Mat(contour1)) );
    double j = fabs( contourArea(cv::Mat(contour2)) );
    return ( i > j );  // sorts in descending order
    return true;
}

//int* findLargeContourAreas(vector<vector<Point>> _contours) {
//    int largestArea = 0, pos[5];
//    sort(_contours.begin(), _contours.end(), compareContourAreas);
//    pos.
//    return pos;
//}

void createAWindow() {
    namedWindow("Threshold Image", CV_WINDOW_OPENGL);
}

void destroyTheWindow() {
    destroyWindow("Threshold Image");
}