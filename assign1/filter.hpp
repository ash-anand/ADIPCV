#ifndef FILTER_H
#define FILTER_H

#include "headers.hpp"
#include <iostream>

void GaussianNoise(const cv::Mat src,cv::Mat &dst, double mean, double stdDev){
	cv::Mat mGaussian_noise = cv::Mat(src.size(),CV_8UC3);
    cv::randn(mGaussian_noise,cv::Scalar::all(mean),cv::Scalar::all(stdDev));
    cv::Mat T;
    T = src.clone();
    for (int row = 0; row < src.rows; ++row)
    {
    	for (int col = 0; col < src.cols; ++col)
    	{
    		for (int i = 0; i < src.channels(); ++i)
    		{
    			int temp = src.at<Pixel>(row,col)[i] + mGaussian_noise.at<Pixel>(row,col)[i];
    			temp = (temp < 0)?0:temp;
    			T.at<Pixel>(row,col)[i] = (temp > 255)?255:temp;
    		}
    	}
    }
    dst = T.clone();
}

cv::Mat getGaussian(int _x, int _y,float stdDev){
	cv::Mat kernel(_x,_y,CV_32FC1);
	int x = _x/2;
	int y = _y/2;

	double r, s = 2.0*stdDev*stdDev;
	double sum = 0.0;

	for(int i = -x; i <= x; i++){
		for(int j = -y; j <= y; j++){
			r = sqrt(i*i + j*j);
			kernel.at<double>(j+2,i+2) = exp(-(r*r)/s)/(CV_PI*s);
			sum += kernel.at<double>(j+2,i+2);
		}
	}

	kernel /= sum;
	// for (int i = 0; i < kernel.rows; ++i)
	// {
	// 	for (int j = 0; j < kernel.cols; ++j)
	// 	{
	// 		kernel.at<double>(j,i) /= sum;
	// 	}
	// }
	return kernel;
}

void GaussianBlur(const cv::Mat src,cv::Mat &dst,int x, int y , float stdDev){
	cv::Mat kernel = getGaussian(x,y,stdDev).clone();
	for (int i = 0; i < src.rows; ++i)
	{
		for (int j = 0; j < src.cols; ++j)
		{
			for(int k = 0; k < kernel.rows; k++){
				for(int l = 0; l < kernel.cols; l++){
					
				}
			}
		}
	}
}

#endif