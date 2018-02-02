#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/stitching.hpp"
#include <vector>
#include <iostream>

using namespace std;

cv::Mat img;
std::vector<cv::Mat> out;
bool finished = false;
std::vector<cv::Point2f> vertices;

float getRealAspectRatio() {

    double u0 = img.cols/2;
    double v0 = img.rows/2;
    double m1x = vertices[0].x - u0;
    double m1y = vertices[0].y - v0;
    double m2x = vertices[1].x - u0;
    double m2y = vertices[1].y - v0;
    double m3x = vertices[3].x - u0;
    double m3y = vertices[3].y - v0;
    double m4x = vertices[2].x - u0;
    double m4y = vertices[2].y - v0;

    double k2 = ((m1y - m4y)*m3x - (m1x - m4x)*m3y + m1x*m4y - m1y*m4x) /
            ((m2y - m4y)*m3x - (m2x - m4x)*m3y + m2x*m4y - m2y*m4x) ;

    double k3 = ((m1y - m4y)*m2x - (m1x - m4x)*m2y + m1x*m4y - m1y*m4x) /
            ((m3y - m4y)*m2x - (m3x - m4x)*m2y + m3x*m4y - m3y*m4x) ;

    double f_squared =
            -((k3*m3y - m1y)*(k2*m2y - m1y) + (k3*m3x - m1x)*(k2*m2x - m1x)) /
                    ((k3 - 1)*(k2 - 1)) ;

    double whRatio = sqrt(
            (pow((k2 - 1),2) + pow((k2*m2y - m1y),2)/f_squared + pow((k2*m2x - m1x),2)/f_squared) /
                    (pow((k3 - 1),2) + pow((k3*m3y - m1y),2)/f_squared + pow((k3*m3x - m1x),2)/f_squared)
    ) ;

    if (k2==1 && k3==1 ) {
        whRatio = sqrt(
                (pow((m2y-m1y),2) + pow((m2x-m1x),2)) /
                        (pow((m3y-m1y),2) + pow((m3x-m1x),2)));
    }

    return (float)(whRatio);
}

#endif