#ifndef GRAY_HPP
#define GRAY_HPP

#include "headers.hpp"

void grayP(Pixel &pixel){
	int temp;
	temp = pixel[0]*0.114 + pixel[1]*0.587 + pixel[2]*0.299;
	pixel[0] = temp;
	pixel[1] = temp;
	pixel[2] = temp;
}

void BGR2GRAY(cv::Mat src,cv::Mat &dst){
	dst = src.clone();
	dst.forEach<Pixel>(
		[](Pixel &pixel,const int*position) -> void
		{
			grayP(pixel);
		}
	);
}

#endif