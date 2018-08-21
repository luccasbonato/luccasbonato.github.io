#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <stdlib.h>

using namespace cv;
using namespace std;

int main(int, char**){
  Mat image, aux1, aux2, image2;

  image= imread("biel.png",CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data)
    cout << "nao abriu biel.png" << endl;

  namedWindow("janela",WINDOW_AUTOSIZE);

//Guardando os quadrantes
  Mat Q1(image, Rect(0,0,(image.size().width/2),(image.size().height/2)));
  Mat Q2(image, Rect((image.size().height/2),0,(image.size().width/2),(image.size().height/2)));
  Mat Q3(image, Rect(0,(image.size().height/2),(image.size().width/2),(image.size().height/2)));
  Mat Q4(image, Rect((image.size().height/2),(image.size().height/2),(image.size().width/2),(image.size().height/2)));

//Fixando os quadrantes um com outro para formar a imagem
  hconcat(Q4, Q3, aux1);
  hconcat(Q2, Q1, aux2);
  vconcat(aux1, aux2, image2);

  imshow("janela", image2);
  imwrite("trocada.png", image2);  
  waitKey();
  return 0;
}
