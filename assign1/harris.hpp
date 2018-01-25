#ifndef HARRIS_HPP
#define HARRIS_HPP 

#include "headers.hpp"

void harris(const cv::Mat src, cv::Mat &dst){
	cv::Mat I_x,I_y,I_x2,I_y2,I_xy,mtrace,x2y2,xy;
	cv::Sobel(src, I_x, CV_32FC1 , 1, 0, 3, cv::BORDER_DEFAULT);
    cv::Sobel(src, I_y, CV_32FC1 , 0, 1, 3, cv::BORDER_DEFAULT);

    cv::pow(I_x,2.0,I_x2);
    cv::pow(I_y,2.0,I_y2);
    multiply(I_x,I_y,I_xy);

    cv::GaussianBlur(I_x2,I_x2,cv::Size(7,7),2.0,0.0,cv::BORDER_DEFAULT);
    cv::GaussianBlur(I_y2,I_y2,cv::Size(7,7),0.0,2.0,cv::BORDER_DEFAULT);
    cv::GaussianBlur(I_xy,I_xy,cv::Size(7,7),2.0,2.0,cv::BORDER_DEFAULT);

    multiply(I_x2,I_y2,x2y2);
    multiply(I_xy,I_xy,xy);
    cv::pow((I_x2 + I_y2),2.0,mtrace);
    dst = (x2y2 - xy) - 0.04 * mtrace;

    cv::normalize( dst, dst, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat() );
    cv::convertScaleAbs( dst, dst );
}
#endif