#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define MAX_GAMA_H  200.0
#define MAX_GAMA_L  200.0
#define MAX_D0      200.0
#define MAX_C       200.0

using namespace cv;
using namespace std;

int gama_H;
int gama_L;
int d0;
int c;

int dft_M, dft_N;
Mat filter, tmp;

void homomorfico(int, void*){
  float _d0 =    ((float) 2*d0/ (float) MAX_D0) * sqrt( dft_M*dft_M + dft_N*dft_N )/2;
  float _gamaL = ((float) 10*gama_L)/ (float) MAX_GAMA_L;
  float _gamaH = ((float) 10*gama_H)/ (float) MAX_GAMA_H;
  float _c =     ((float) 10*c)/ (float) MAX_C;

  double n;
  double dGamas = _gamaH - _gamaL;
  double d02 = _d0*_d0;

  for(int i=0; i<dft_M; i++){
    for(int j=0; j<dft_N; j++){
      n = _c * ((i - dft_M/2)*(i - dft_M/2) + (j - dft_N/2)*(j - dft_N/2));
      tmp.at<float>(i,j) = dGamas * (1 - exp(-n/d02)) + _gamaL;
    }
  }
  Mat comps[]= {tmp, tmp};
  merge(comps, 2, filter);
}

// troca os quadrantes da imagem da DFT
void deslocaDFT(Mat& image )
{
  Mat aux, A, B, C, D;

  // se a imagem tiver tamanho impar, recorta a regiao para
  // evitar cópias de tamanho desigual
  image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
  int cx = image.cols/2;
  int cy = image.rows/2;
  
  // reorganiza os quadrantes da transformada
  // A B   ->  D C
  // C D       B A
  A = image(Rect(0, 0, cx, cy));
  B = image(Rect(cx, 0, cx, cy));
  C = image(Rect(0, cy, cx, cy));
  D = image(Rect(cx, cy, cx, cy));

  A.copyTo(aux);  D.copyTo(A);  aux.copyTo(D); // A <-> D
  C.copyTo(aux);  B.copyTo(C);  aux.copyTo(B); // C <-> B
}

int main(int , char**){
  VideoCapture cap;   
  Mat imaginaryInput, complexImage, multsp;
  Mat padded, mag;
  Mat image, imagegray; 
  Mat_<float> realInput, zeros;
  vector<Mat> planos;

  cap.open(0);
  if(!cap.isOpened()) return -1;

  cap >> image;

  dft_M = getOptimalDFTSize(image.rows);
  dft_N = getOptimalDFTSize(image.cols);

  copyMakeBorder(image, padded, 0, dft_M - image.rows, 0, dft_N - image.cols, BORDER_CONSTANT, Scalar::all(0));

  zeros = Mat_<float>::zeros(padded.size());

  complexImage = Mat(padded.size(), CV_32FC2, Scalar(0));

  filter = complexImage.clone();

  tmp = Mat(dft_M, dft_N, CV_32F);

  char title[25];
  namedWindow("saida", 1);

  sprintf(title, "Gama H x %.1lf", MAX_GAMA_H);
  createTrackbar(title, "saida",   &gama_H, MAX_GAMA_H, homomorfico);

  sprintf(title, "Gama L x %.1lf", MAX_GAMA_L);
  createTrackbar(title, "saida",   &gama_L, MAX_GAMA_L, homomorfico);
  
  sprintf(title, "D0 x %.1lf",  MAX_D0);
  createTrackbar(title, "saida", &d0, MAX_D0, homomorfico);

  sprintf(title, "c x %.1lf",   MAX_C);
  createTrackbar(title, "saida", &c, MAX_C, homomorfico);

  homomorfico(0, 0);

  for(;;){
    cap >> image;
    cvtColor(image, imagegray, CV_BGR2GRAY);
    imshow("original", imagegray);

    copyMakeBorder(imagegray, padded, 0, dft_M - image.rows, 0, dft_N - image.cols, BORDER_CONSTANT, Scalar::all(0));

    planos.clear();
    realInput = Mat_<float>(padded);
    planos.push_back(realInput);
    planos.push_back(zeros);


    realInput = realInput + Scalar::all(1);
    log(realInput,realInput);

    merge(planos, complexImage);

    dft(complexImage, complexImage);

    deslocaDFT(complexImage);

    mulSpectrums(complexImage,filter,complexImage,0);

    planos.clear();
    split(complexImage, planos);
 
    merge(planos, complexImage);

    deslocaDFT(complexImage);

    idft(complexImage, complexImage);

    planos.clear();

    split(complexImage, planos);

    normalize(planos[0], planos[0], 0, 1, CV_MINMAX);
    exp(planos[0],planos[0]);
    normalize(planos[0], planos[0], 0, 1, CV_MINMAX);
    imshow("saida", planos[0]);

    if(waitKey(30) >= 0){
      break;
    }
  }
  return 0;
} 
