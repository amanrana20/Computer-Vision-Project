#include "TrackerBar.hpp"

// GLOBAL VARIABLES
extern int thresholdValue;

// Hue values range
extern int h_min;
extern int h_max;

// saturation values range
extern int s_min;
extern int s_max;

// Value values range
extern int v_min;
extern int v_max;

// GLOBAL VARIABLES END

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