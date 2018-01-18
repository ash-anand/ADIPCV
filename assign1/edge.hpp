#ifndef EDGE_H
#define EDGE_H
#include "headers.hpp"

void filter(cv::Mat src, cv::Mat &dst, int kernel[3][3]){
	for(int i = 0; i < src.rows; i++)
		for(int j = 0; j < src.cols; j++){
			int sum = 0;
			for(int k = -1; k <= 1; k++)
				for(int l = -1; l <= 1; l++){
					int pic_i = i + k;
					int pic_j = j + l;
					if(pic_i < 0 || pic_j < 0 || pic_i > src.rows - 1 || pic_j > src.cols - 1)
						continue;
					sum += kernel[k+1][l+1]*src.at<uchar>(pic_i,pic_j);
				}
			sum = abs(sum);
			dst.at<uchar>(i,j) = (sum>255)?255:sum;
		}
}

void Sobel(const cv::Mat src, cv::Mat &dst, cv::Mat &dstX,cv::Mat &dstY){
	int X[3][3] = {{-1,0,1},
					{-2,0,2},
					{-1,0,1}};
	int Y[3][3] = {{-1,-2,-1},
					{0,0,0},
					{1,2,1}};
	cv::Mat mX(src.size(),src.type()),mY(src.size(),src.type()),mXY(src.size(),src.type());
	filter(src,mX,X);
	filter(src,mY,Y);
	for(int i = 0; i < src.rows; i++)
		for(int j = 0; j < src.cols; j++){
			mXY.at<uchar>(i,j) = sqrt(mX.at<uchar>(i,j)*mX.at<uchar>(i,j) + mY.at<uchar>(i,j)*mY.at<uchar>(i,j));
		}
	dst = mXY.clone();
	dstX = mX.clone();
	dstY = mY.clone();
}
#endif