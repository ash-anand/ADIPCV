#include "gray.hpp"
#include "filter.hpp"
#include "edge.hpp"
#include "log.hpp"
#include "enhance.hpp"
#include "harris.hpp"
#include "segment.hpp"

static void show_usage(std::string name)
{
    std::cerr << "Usage: " << name << " <option(s)> [Option Number]\n"
              << "\tOptions:\n"
              << "\t-h,--help\t\tShow this help message\n"
              << "\t-q [number(s)]\t\tSpecify the question bits of assignment.\n"
              << "\tQuestion Numbers\n"
              << "\t1.Reading and Conversion to gray\n"
              << "\t2.Noise and Filtering\n"
              << "\t3.Edge Detections\n"
              << "\t4.LoG Operation\n"
              << "\t5.Text and Foreground Extraction\n"
              << "\t6.Image Enhancement\n"
              << "\t7.Harris Corner Detection"
              << std::endl;
}

cv::Mat image,gray, gBlur;

void Q1(){
	//Gray Conversion
	cv::Mat g;
	BGR2GRAY(image,g);
	cv::imshow("Gray Implementation",g);
	cv::imshow("Gray Library Function",gray);
	cv::waitKey(0);
}

void Q2(){
	//Noise 
	cv::Mat noise,mBlur,gBlur;
	GaussianNoise(gray,noise,0,20);
	imshow("Noise",noise);

	GaussianBlur(gray,gBlur,3,3,4);
	cv::imshow("Gaussian Blur Implementation",gBlur);
	cv::GaussianBlur( gray, gBlur, cv::Size(3,3),4,4);
	cv::imshow("Gaussian Blur Library",gBlur);

	MedianBlur(gray,mBlur,3,3);
	cv::imshow("Median Blur Implementation",mBlur);
	cv::medianBlur(gray,mBlur,3);
	cv::imshow("Median Blur Library",mBlur);

	std::cout<<"\nPSNR of Noise : "<<PSNR(gray,noise)<<std::endl;
	std::cout<<"\nPSNR of Gaussian Blur : "<<PSNR(gray,gBlur)<<std::endl;
	std::cout<<"\nPSNR of Median Blur : "<<PSNR(gray,mBlur)<<std::endl;
	cv::waitKey(0);
	cv::destroyAllWindows();
}

void Q3(){
	//Edge
	cv::GaussianBlur( gray, gBlur, cv::Size(3,3),4,4);
	cv::Mat sobel,sobelX,sobelY;

	Prewitt(gBlur,sobel,sobelX,sobelY);
	imshow("Prewitt X-gradient",sobelX);
	imshow("Prewitt Y-gradient",sobelY);
	imshow("Prewitt Gradient Magnitude",sobel);

	int thres = 63;
	sobel.forEach<uchar>(
		[&thres](uchar &pixel,const int*position) -> void
		{
			if(pixel < thres)
				pixel = 0;
			else 
				pixel = 255;
		}
	);
	imshow("Edge Using Prewitt",sobel);

	cv::waitKey(0);
	cv::destroyAllWindows();
	cv::waitKey(30);
	Sobel(gBlur,sobel,sobelX,sobelY);
	imshow("Sobel X-gradient Implementation",sobelX);
	imshow("Sobel Y-gradient Implementation",sobelY);
	imshow("Sobel Gradient Magnitude Implementation",sobel);

	cv::Sobel(gray, sobelX, CV_16S, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT );
	cv::Sobel(gray, sobelY, CV_16S, 0, 1, 3, 1, 0, cv::BORDER_DEFAULT );
	convertScaleAbs( sobelX, sobelX );
	convertScaleAbs( sobelY, sobelY );
	addWeighted( sobelX, 0.5, sobelY, 0.5, 0, sobel );
	imshow("Sobel X-gradient Library",sobelX);
	imshow("Sobel Y-gradient Library",sobelY);
	imshow("Sobel Gradient Magnitude Library",sobel);

	sobel.forEach<uchar>(
		[&thres](uchar &pixel,const int*position) -> void
		{
			if(pixel < thres)
				pixel = 0;
			else 
				pixel = 255;
		}
	);
	imshow("Edge Using Sobel",sobel);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

void Q4(){
	//Laplacian
	cv::Mat lapMat;
	cv::GaussianBlur( gray, gBlur, cv::Size(3,3),4,4);
	LoG(gBlur,lapMat,7,7,5);
	imshow("Laplacian of Gaussian",lapMat);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

void Q5(){
	//Background Filter 
	cv::Mat back,letter;
	thresNextract(image,back,letter);
	cv::imshow("Extracted Foreground",back);
	cv::imshow("Letter Detection",letter);
	cv::waitKey(0);
	cv::destroyAllWindows();
}

void Q6(){
	//Image Enhancement
	cv::Mat hist;
	cv::GaussianBlur( gray, gBlur, cv::Size(3,3),4,4);
	histeqi(gBlur,hist);
	// alpha(gBlur,hist);
	cv::imshow("Histogram Equalisation Implementation",hist);

	cv::equalizeHist(gBlur,hist);
	cv::imshow("Histogram Equalisation Library",hist);

	cv::waitKey(0);
	cv::destroyAllWindows();
}

void Q7(){
	//Harris
	cv::Mat M_harris;
	cv::GaussianBlur( gray, gBlur, cv::Size(3,3),4,4);
	harris(gBlur,M_harris);
	cv::imshow("Harris Implementation",M_harris);

	cv::Mat dst, dst_norm, dst_norm_scaled;
  	dst = cv::Mat::zeros( gray.size(), CV_32FC1 );
	cv::cornerHarris( gray, dst, 2, 3, 0.04, cv::BORDER_DEFAULT );
	/// Normalizing
	cv::normalize( dst, dst_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat() );
	cv::convertScaleAbs( dst_norm, dst_norm_scaled );
	cv::imshow("Harris Library",dst_norm_scaled);

	cv::waitKey(0);
	cv::destroyAllWindows();
}

int main(int argc, char* argv[]){

	if (argc < 2) {
        show_usage(argv[0]);
        return 1;
    }

    image = cv::imread("Plaque.jpg",1);
	gray = cv::imread("Plaque.jpg",0);
	cv::imshow("Original",image);
    std::string arg = argv[1];
    if ((arg == "-h") || (arg == "--help")) {
        show_usage(argv[0]);
        return 0;
    } else{
    	 if ((arg == "-q")) {
    	 	if(argc < 3){
    	 		show_usage(argv[0]);
    	 		return 1;
    	 	}

        	int choice = atoi(argv[2]);
        	switch(choice){
        		case 1:
        			Q1();
        			break;
        		case 2:
        			Q2();
        			break;
        		case 3:
        			Q3();
        			break;
        		case 4:
        			Q4();
        			break;
        		case 5:
        			Q5();
        			break;
        		case 6:
        			Q6();
        			break;
        		case 7:
        			Q7();
        			break;

        	}
    	}
    }
	return 0;
}