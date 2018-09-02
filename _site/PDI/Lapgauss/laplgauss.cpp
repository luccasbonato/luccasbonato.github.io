#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argvc, char** argv){
  VideoCapture video;
  float gauss[] = {1,2,1,
		   2,4,2,
		   1,2,1};
  float laplacian[]={0 ,-1,0,
		     -1,4 ,-1,
		     0 ,-1,0};

  Mat cap, frame, frame32f, frameF1, frameF2;
  Mat mask(3,3,CV_32F);
  Mat result;
  char key;
  
  video.open(0); 
  if(!video.isOpened()) 
    return -1;

  namedWindow("filtroespacial",1);

  for(;;){
    video >> cap; 
    cvtColor(cap, frame, CV_BGR2GRAY);
    flip(frame, frame, 1);
    imshow("original", frame);
    frame.convertTo(frame32f, CV_32F);

    //aplicando a mascara de gauss
    mask = Mat(3, 3, CV_32F, gauss);

    filter2D(frame32f, frameF1, frame32f.depth(), mask, Point(1,1), 0);
    
    //aplicando a mascara laplaciana
    mask = Mat(3, 3, CV_32F, laplacian);

    filter2D(frameF1, frameF2, frame32f.depth(), mask, Point(1,1), 0);

    //obs: obs o resultado dessa operação é a mesma coisa de aplica uma unica mascara cujo é o resultado da convolução da gaussiana com a laplaciana
    
    frameF2.convertTo(result, CV_8U);

    imshow("filtroespacial", result);

    key = (char) waitKey(10);
    if( key == 27 ){
    imwrite("frame.png", frame);
    imwrite("filtrada.png", result);

    break; // esc pressed!
    }
  }
  return 0;
}
