#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <vector>
#include <math.h>

using namespace std;
using namespace cv;

Mat img,tem,orig;
int point = 0;
float lineA[3],lineB[3];
std::vector<Point> points;
int cCount = 0;
bool finished = false;
float P[4][3];

float getDistance(Point x, Point y){
	return sqrt(pow(x.x-y.x,2) + pow(x.y-y.y,2));
}

void cross(float vect_A[], float vect_B[], float cross_P[])
 
{
    cross_P[0] = vect_A[1] * vect_B[2] - vect_A[2] * vect_B[1];
    cross_P[1] = vect_A[0] * vect_B[2] - vect_A[2] * vect_B[0];
    cross_P[2] = vect_A[0] * vect_B[1] - vect_A[1] * vect_B[0];
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	int *i = (int *)userdata;
	if  ( event == EVENT_LBUTTONDOWN ){
    	cCount++;
    	if(cCount%2 == 0){
    		line(img,points.back(), Point(x,y), Scalar(0,255,0));
    		cout<<"Distance between "<<Point(x,y)<<" and "<<points.back()<<" is "<<getDistance(points.back(),Point(x,y))<<" pixels\n";
    		if(cCount == 4){
    			float a1[] = {points.back().x,points.back().y,1};
    			float a2[] = {x,y,1};
    			cross(a1,a2,lineB);
    			float a[3];
    			cross(lineA,lineB,a);
    			P[*i][0] = a[0]/a[2];
    			P[*i][1] = a[1]/a[2];
    			cout<<"Vanishing Point : "<<P[*i][0]<<" "<<P[*i][1]<<endl;
    			cCount = 0;
    			finished = true;
    			img = orig.clone();
    			tem = orig.clone();
    		}
    		else{
    			float a1[] = {points.back().x,points.back().y,1};
    			float a2[] = {x,y,1};
    			cross(a1,a2,lineA);
    		}
    		points.push_back(Point(x,y));
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
	cout<<"Select parallel lines in X-axis\n";
	waitKey(50);
	while(!finished and point != 2){
		waitKey(12);
		imshow("Image",tem);
	}
	point = 0;
	finished = false;
	flag = 1;
	cout<<"Select parallel lines in Y-axis\n";
	waitKey(50);
	while(!finished and point != 2){
		waitKey(12);
		imshow("Image",tem);
	}
	point = 0;
	finished = false;
	flag = 2;
	cout<<"Select parallel lines in Z-axis\n";
	waitKey(50);
	while(!finished and point != 2){
		waitKey(12);
		imshow("Image",tem);
	}
	return 0;
}