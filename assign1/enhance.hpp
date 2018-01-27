#ifndef ENHANCE_HPP
#define ENHANCE_HPP

#include "headers.hpp"

void histeqi(const cv::Mat src, cv::Mat &dst){
	int sz = src.rows*src.cols;
	std::vector<int> histList(256,0);
	dst = src.clone();
	src.forEach<uchar>(
		[&histList](uchar pixel,const int*Pos) -> void {
			histList[pixel]++;
		}
	);
	std::vector<int> histDistribution(256,0);
	int sum = 0;
	for (int i = 0; i < histList.size(); ++i)	{
		sum += histList[i];
		histDistribution[i] = sum;
	}

	dst.forEach<uchar>(
		[histDistribution,sz](uchar &pixel,const int *pos) -> void{
			pixel = histDistribution[pixel]*255/sz;
		}
	);
}

void alpha(cv::Mat src, cv::Mat &dst){
	src.convertTo(src,CV_32F);
	
}
#endif