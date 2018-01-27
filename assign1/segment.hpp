#ifndef SEGMENT_HPP
#define SEGMENT_HPP

#include "headers.hpp"
#include "opencv2/features2d/features2d.hpp"

std::vector<cv::Rect> detectLetters(cv::Mat img)
{
    std::vector<cv::Rect> boundRect;
    cv::Mat img_gray, img_sobel, img_threshold, element;
    if(img.channels() == 3)
      cv::cvtColor(img,img_gray,cv::COLOR_BGR2GRAY);
    else
      img_gray = img.clone();
    cv::Sobel(img_gray, img_sobel, CV_8U, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
    cv::threshold(img_sobel, img_threshold, 0, 255, CV_THRESH_OTSU+CV_THRESH_BINARY);
    element = getStructuringElement(cv::MORPH_RECT, cv::Size(17, 3) );
    cv::morphologyEx(img_threshold, img_threshold, CV_MOP_CLOSE, element); //Does the trick
    std::vector< std::vector< cv::Point> > contours;
    cv::findContours(img_threshold, contours, 0, 1); 
    std::vector<std::vector<cv::Point> > contours_poly( contours.size() );
    for( int i = 0; i < contours.size(); i++ )
        if (contours[i].size()>100)
        { 
            cv::approxPolyDP( cv::Mat(contours[i]), contours_poly[i], 3, true );
            cv::Rect appRect( boundingRect( cv::Mat(contours_poly[i]) ));
            if (appRect.width>appRect.height) 
                boundRect.push_back(appRect);
        }
    return boundRect;
}

void thresNextract(cv::Mat src, cv::Mat &extract, cv::Mat &letters){
	cv::Mat img = src.clone();
    cv::Mat mask(img.size(),CV_8UC1);
    mask *= 0;

    cv::floodFill(src,cv::Point(420,100), cv::Scalar(0,0,255)  ,0, cv::Scalar(5,5,5),cv::Scalar(5,5,5),4);
    cv::floodFill(src,cv::Point(220,70), cv::Scalar(0,0,255) ,0, cv::Scalar(5,5,5),cv::Scalar(5,5,15),4);
    cv::floodFill(src,cv::Point(420,70), cv::Scalar(0,0,255) ,0, cv::Scalar(5,5,5),cv::Scalar(5,5,15),4);
    cv::floodFill(src,cv::Point(320,80), cv::Scalar(0,0,255) ,0, cv::Scalar(5,5,5),cv::Scalar(5,5,15),4);
    
    src.forEach<cv::Vec3b>(
      [&mask](cv::Vec3b &pixel,const int *pos) -> void{
        if(pixel == cv::Vec3b(0,0,255)){
          mask.at<uchar>(pos[0],pos[1]) = 255;
        }
      }
    );

    int morph_size = 2;
    cv::Mat element = getStructuringElement( cv::MORPH_RECT, 
      cv::Size( 2*morph_size + 1, 2*morph_size+1 ), 
      cv::Point( morph_size, morph_size ) ); 
    cv::morphologyEx( mask, mask, cv::MORPH_CLOSE, element, cv::Point(-1,-1), 5 ); 
    
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(mask,contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

    int biggestContourIdx = -1;
    float biggestContourArea = 0;
    extract = cv::Mat::zeros( mask.size(), CV_8UC3 );
    
    for( int i = 0; i< contours.size(); i++ )
    {
        cv::Scalar color = cv::Scalar(100, 50, 60);
        drawContours( extract, contours, i, color, 1, 8, hierarchy, 0, cv::Point() );

        float ctArea= cv::contourArea(contours[i]);
        if(ctArea > biggestContourArea)
        {
            biggestContourArea = ctArea;
            biggestContourIdx = i;
        }
    }

    if(biggestContourIdx < 0)
    {
        return;
    }

 	cv::RotatedRect boundingBox = cv::minAreaRect(contours[biggestContourIdx]);
    cv::Point2f corners[4];

    boundingBox.points(corners);

    int w = abs(corners[2].x - corners[0].x) , h = abs(corners[0].y - corners[1].y);
    cv::Rect myROI(corners[1].x ,corners[1].y,w,h);

    img.copyTo(extract,mask);

    letters = extract(myROI).clone();

    cv::Point2f pc(letters.cols/2., letters.rows/2.);
    cv::Mat r = cv::getRotationMatrix2D(pc, 2.7, 1.0);
    
    cv::warpAffine(letters, letters, r, letters.size());
    letters = letters(cv::Rect(40,60,w - 50, h - 130)).clone();
    
    std::vector<cv::Rect> letterBBoxes1=detectLetters(letters);

    for(int i=0; i< letterBBoxes1.size(); i++)
        cv::rectangle(letters,letterBBoxes1[i],cv::Scalar(0,255,0),1,8,0);
}



#endif