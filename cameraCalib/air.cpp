#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <vector>
#include <math.h>

using namespace std;
using namespace cv;

Mat img,tem,orig;
std::vector<Point> points;
int cCount;
bool finished = false;
Point center(-1,-1);

float getDistance(Point x, Point y){
	return sqrt(pow(x.x-y.x,2) + pow(x.y-y.y,2));
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	int *i = (int *)userdata;
	if  ( event == EVENT_LBUTTONDOWN ){
		if(!*i){
			center = Point(x,y);
			finished = true;
			return;
		}
    	cCount++;
    	if(cCount%2 == 0){
    		line(img,points.back(), Point(x,y), Scalar(0,255,0));
    		cout<<"Distance between "<<Point(x,y)<<" and "<<points.back()<<" is "<<getDistance(points.back(),Point(x,y))<<" pixels\n";
    		points.push_back(Point(x,y));
    		finished = true;
    	}
    	else{
    		points.push_back(Point(x,y));
    		circle(img,Point(x,y),3, Scalar(255,0,0), 1);
    	}
    	tem = img.clone();
    }
    if  ( event == EVENT_RBUTTONDOWN ){
    	img = orig.clone();
    	tem = orig.clone();
    }
    else if ( event == EVENT_MOUSEMOVE ){
    	if(cCount%2 != 0){
    		tem = img.clone();
    		line(tem,points.back(), Point(x,y), Scalar(0,128,255));
    	}
	}
}

int main(int argc, char **argv){

	const int dis = 150;
	const float wheel2wheel = 28.660344;
	const float tailHt = 16.8402;
	const float fullSpan = 66.81216;
	const float bodyHeight = 6.089904;
	const int z = 150;
	int flag = 0;
	
	img = imread(argv[1],1);
	tem = img.clone();
	orig = img.clone();
	std::vector<std::vector<Point2f> > correspondences;

	// circle(tem,Point(img.cols/2,img.rows/2),50, Scalar(0,0,255), -1);

	namedWindow("Image", 1);
	setMouseCallback("Image", CallBackFunc, &flag);

	//Center choosing
	cout<<"Select Center..\n";
	waitKey(50);
	while(!finished){
		waitKey(12);
		imshow("Image",tem);
	}
	cout<<"Centre : "<<center<<endl;
	circle(orig,center,2, Scalar(0,0,255), -1);
	img = orig.clone();
	tem = img.clone();
	finished = false;
	flag = 1;
	cout<<"Draw line from nose tip to tail tip in X-axis..\n";
	waitKey(50);
	while(!finished){
		waitKey(12);
		imshow("Image",tem);
	}
	Point2f picFullSpan = Point2f(getDistance(center,points.back()),0.0);
	vector<Point2f> v;
	v.push_back(picFullSpan);
	v.push_back(points.back() - center);
	correspondences.push_back(v);

	return 0;
}