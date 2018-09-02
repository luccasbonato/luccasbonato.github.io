#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int top_slider = 10;
int top_slider_max = 200;

char TrackbarName[50];

Mat image, border;

void on_trackbar_canny(int, void*){
  Canny(image, border, top_slider, 3*top_slider);
  imshow("canny", border);
}

int main(int argc, char**argv){

  //image= imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);

  VideoCapture cap;

  cap.open(0);
  if(!cap.isOpened()) return -1;

  sprintf( TrackbarName, "Threshold inferior");

  namedWindow("canny",1);
  createTrackbar( TrackbarName, "canny",
                &top_slider,
                top_slider_max,
                on_trackbar_canny );

  while(1){
    cap >> image;
    on_trackbar_canny(top_slider, 0 );
    if(waitKey(30) >= 0){
      break;
    }
  }
  imwrite("cannyborders.png", border);
  return 0;
}
