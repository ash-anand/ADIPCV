#include "transform.hpp"

using namespace cv;

void CallBackFunc(int event,int x,int y,int flags,void* userdata)
{
  int index = *(int*)userdata;
   if(event==EVENT_RBUTTONDOWN){
      if(vertices.size()<3){
         cout << "Four Points are needed!"<<vertices.size() << endl;
         return;
      }
      finished=true;

      return;
   }
   if(event==EVENT_LBUTTONDOWN){
      if(vertices.size()==0 or vertices.size() == 4){
         // First click - just draw point
        vertices.clear();
      } else {
         // Second, or later click, draw line to previous vertex
         line(img,Point(x,y),vertices[vertices.size()-1],Scalar(0,0,0));
      }
      int rad = img.cols * img.rows;
      rad *= 0.000002;
      circle(img,Point(x,y),rad,Scalar(0,55,255),rad);
      vertices.push_back(Point2f(x,y));
      return;
   }
}


int main(int argc, char **argv)
{         
    // Lambda Matrix
    Mat lambda( 2, 4, CV_32FC1 );
    vertices.reserve(4);
    out.reserve(3);
    //Input and Output Image;
    Mat input;
    //Load the image
    input = imread( argv[1], 1 );
    // resize(input,input,Size(),0.25,0.25);
    //Create a window
    int height;
    for(int i = 0; i < 3; i++){
      img = input.clone();
      namedWindow("ImageDisplay",CV_WINDOW_NORMAL);
      if(i == 0)
        std::cout<<"Select Centre Pic\n";
      else 
        if(i == 1)
          std::cout<<"Select Left Pic\n";
        else
          std::cout<<"Select Right Pic\n";

      cv::Point2f inputQuad[4]; 
      cv::Point2f outputQuad[4];
      // Register a mouse callback
      setMouseCallback("ImageDisplay",CallBackFunc,&i);

      while(!finished){
          imshow("ImageDisplay",img);
          resizeWindow("ImageDisplay", 1200, 800);
          waitKey(30);
      }
      destroyAllWindows();

      float aspect = getRealAspectRatio();
      if(!i)
        height = vertices[2].y - vertices[1].y ;
      cout<<aspect;
      Mat output(height,(aspect+0.1)*height,CV_8UC3,Scalar::all(0));
      // Set the lambda matrix the same type and size as input
      lambda = Mat::zeros( input.rows, input.cols, input.type() );
   
      // The 4 points that select quadilateral on the input , from top-left in clockwise order
      // These four pts are the sides of the rect box used as input 
      inputQuad[0] = vertices[0];
      inputQuad[1] = vertices[1];
      inputQuad[2] = vertices[2];
      inputQuad[3] = vertices[3]; 
      // The 4 points where the mapping is to be done , from top-left in clockwise order
      outputQuad[0] = Point2f( 0,0 );
      outputQuad[1] = Point2f( output.cols - 1,0);
      outputQuad[2] = Point2f( output.cols,output.rows);
      outputQuad[3] = Point2f( 0,output.rows  );
   
      // Get the Perspective Transform Matrix i.e. lambda 
      lambda = getPerspectiveTransform( inputQuad, outputQuad );
      // Apply the Perspective Transform just found to the src image
      warpPerspective(input,output,lambda,output.size() );
      out.push_back(output.clone());
      finished = false;
    }
 
    //Display input and output
    namedWindow("Image",CV_WINDOW_NORMAL);
    imshow("Image",input);
    resizeWindow("Image", 1200, 800);
    namedWindow("Output",CV_WINDOW_NORMAL);
    namedWindow("OutputIn",CV_WINDOW_NORMAL);
    Mat output;
    hconcat(out[1],out[0],output);
    hconcat(output,out[2],output);
    Mat mask(output.size(),CV_8UC1,Scalar(0));
    int frac = 30;
    for (int i = -frac; i < frac ; ++i)
    {
        mask.col(out[1].cols + i) = 255;
        mask.col(output.cols - out[2].cols + i) = 255;
    }

    imshow("Output",output);
    inpaint(output,mask,output,15,INPAINT_TELEA);

    imshow("OutputIn",output);
    resizeWindow("Output", 600, 400);
    resizeWindow("OutputIn", 600, 400);
    waitKey(0);
    return 0;
}