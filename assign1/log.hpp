#ifndef LOG_HPP
#define LOG_HPP 
#include "headers.hpp"

cv::Mat getLaplacian(int _x, int _y,float stdDev){
	cv::Mat kernel(_x,_y,cv::DataType<double>::type);
	int x = _x/2;
	int y = _y/2;

	double r, s = stdDev*stdDev;
	double sum = 0.0;

	for(int i = -x; i <= x; i++){
		for(int j = -y; j <= y; j++){
			r = i*i + j*j;
			kernel.at<double>(j+y,i+x) = exp((-r)/2*s);
			sum += kernel.at<double>(j+y,i+x);
		}
	}

	for(int i = -x; i <= x; i++){
		for(int j = -y; j <= y; j++){
			r = i*i + j*j;
			kernel.at<double>(j+y,i+x) *= (r - 2*s)/(2*CV_PI*s*s*s*sum);
		}
	}

	double z = kernel.at<double>(0,0);
	sum = 0;
	kernel.forEach<double>(
		[z,&sum](double &pix,const int *pos)
		{
			pix -= z;
			sum += pix;
		}
	);
	if(sum < 0) sum *= -1;
	kernel.forEach<double>(
		[sum](double &pix,const int *pos)
		{
			pix /= sum;
		}
	);
	return kernel;
}

void zeroEdge(cv::Mat src,int x, int y){
	cv::Mat z(src.size(),CV_8UC1);
	z = 0;
	for (int i = 0; i < src.rows; ++i){
		for (int j = 0; j < src.cols; ++j){
			int neg = 0, pos = 0;
			for(int k = -x; k <= x; k++){
				for(int l = -y; l <= y; l++){
					if(i+k < 0 || j+l < 0 || i+k > src.rows || j+l > src.cols)
						continue;
					if(src.at<double>(i+k,j+l) > 0)
						pos++;
					else
						if(src.at<double>(i+k,j+l) < 0)
							neg++;
				}
			}
			if(neg > 0 and pos > 0)
				z.at<uchar>(i,j) = 255;
		}
	}
	imshow("Zero Crossing",z);
}

void LoG(const cv::Mat src, cv::Mat &dst, int x, int y, double stdDev){
	cv::Mat kernel = getLaplacian(x,y,stdDev);
	dst.create(src.size(),src.type());
	cv::Mat zero(src.size(),cv::DataType<double>::type);
	for (int i = 0; i < src.rows; ++i){
		for (int j = 0; j < src.cols; ++j){
			double sum = 0;
			for(int k = 0; k < kernel.rows; k++){
				for(int l = 0; l < kernel.cols; l++){
					int pic_i = i + k - kernel.rows/2;
					int pic_j = j + l - kernel.cols/2;
					if( pic_j < 0 || pic_i < 0 || pic_i > src.rows || pic_j > src.cols)
						continue;
					sum += kernel.at<double>(k,l)*src.at<uchar>(pic_i,pic_j);
				}
			}
			dst.at<uchar>(i,j) = abs(sum) - 128;
			zero.at<double>(i,j) = abs(sum) - 128;
		}
	}
	zeroEdge(zero,1,1);
}

#endif