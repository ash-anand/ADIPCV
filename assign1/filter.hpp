#ifndef FILTER_H
#define FILTER_H

#include "headers.hpp"
#include <iostream>
#include <vector>

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
	cv::Mat kernel(_x,_y,cv::DataType<double>::type);
	int x = _x/2;
	int y = _y/2;

	double r, s = 2.0*stdDev*stdDev;
	double sum = 0.0;

	for(int i = -x; i <= x; i++){
		for(int j = -y; j <= y; j++){
			r = i*i + j*j;
			kernel.at<double>(j+y,i+x) = exp((-r)/s);
			sum += kernel.at<double>(j+y,i+x);
		}
	}

	for (int i = 0; i < kernel.rows; ++i)
	{
		for (int j = 0; j < kernel.cols; ++j)
		{
			kernel.at<double>(j,i) /= sum;
		}
	}
	return kernel;
}

void GaussianBlur(const cv::Mat src,cv::Mat &dst,int x, int y , float stdDev){
	cv::Mat kernel = getGaussian(x,y,stdDev).clone();
	dst.create(src.size(),src.type());
	for (int i = 0; i < src.rows; ++i){
		for (int j = 0; j < src.cols; ++j){
			int sum = 0;
			for(int k = 0; k < kernel.rows; k++){
				for(int l = 0; l < kernel.cols; l++){
					int pic_i = i + k - kernel.rows/2;
					int pic_j = j + l - kernel.cols/2;
					if( pic_j < 0 || pic_i < 0)
						continue;
					sum += kernel.at<double>(k,l)*src.at<uchar>(pic_i,pic_j);
				}
			}
			dst.at<uchar>(i,j) = sum;
		}
	}
}

double median(std::vector<int> scores)
{
  double median;
  size_t size = scores.size();

  sort(scores.begin(), scores.end());

  if (size  % 2 == 0)
  {
      median = (scores[size / 2 - 1] + scores[size / 2]) / 2;
  }
  else 
  {
      median = scores[size / 2];
  }

  return median;
}

void MedianBlur(const cv::Mat src, cv::Mat &dst, int _x, int _y){
	dst.create(src.size(),src.type());
	for(int i = 0; i < src.rows; i++)
		for(int j = 0; j < src.cols; j++){
			std::vector<int> temp;
			for(int k = 0; k <= _x; k++)
				for(int l = 0; l <= _y; l++){
					int pic_i = i + k - _x/2;
					int pic_j = j + l - _y/2;
					if( pic_j < 0 || pic_i < 0)
						continue;
					temp.push_back(src.at<uchar>(pic_i,pic_j));
				}
			dst.at<uchar>(i,j) = median(temp);
		}
}

#endif