#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace cv;

#define STEP 5
#define JITTER 3
#define RAIO 3

int main(int argc, char** argv){
  vector<int> yrange;
  vector<int> xrange;

  Mat image, points;

  int width, height, r, g, b;
  int x, y;

  VideoCapture cap;

  cap.open(0);
  if(!cap.isOpened()) return -1;
  srand(time(0));

  while(1){
    cap >> image;

    width=image.size().width;
    height=image.size().height;

    xrange.resize(height/STEP);
    yrange.resize(width/STEP);
  
    iota(xrange.begin(), xrange.end(), 0); 
    iota(yrange.begin(), yrange.end(), 0);

    for(uint i=0; i<xrange.size(); i++){
      xrange[i]= xrange[i]*STEP+STEP/2;
    }

    for(uint i=0; i<yrange.size(); i++){
      yrange[i]= yrange[i]*STEP+STEP/2;
    }

    points = Mat(height, width, CV_8UC3, Scalar(CV_RGB(255,255,255)));

    random_shuffle(xrange.begin(), xrange.end());

    for(auto i : xrange){
      random_shuffle(yrange.begin(), yrange.end());
      for(auto j : yrange){
        x = i+rand()%(2*JITTER)-JITTER+1;
        y = j+rand()%(2*JITTER)-JITTER+1;
        r = image.at<cv::Vec3b>(x,y)[2];
        g = image.at<cv::Vec3b>(x,y)[1];
        b = image.at<cv::Vec3b>(x,y)[0];
        circle(points,
               cv::Point(y,x),
               RAIO,
               CV_RGB(r,g,b),
               -1,
               CV_AA);
      }
    }
    imshow("Saida", points);
    if(waitKey(30) >= 0){
      break;
    }
  }
  imwrite("pontos.jpg", points);
  return 0;
}
