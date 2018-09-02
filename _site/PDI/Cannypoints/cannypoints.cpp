#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>

#define STEP 5
#define JITTER 3

using namespace std;
using namespace cv;

int main(int argc, char**argv){
  //Definição de variaveis que ultilizaremos no programa
  Mat image, border, points;
  int threshold, width, height, x, y, r, g , b, i, j;

  vector<int> yrange;
  vector<int> xrange;

  //ja definimos que o primeiro raio sera de tamanho 5
  int RAIO = 5;
  
  //Lemos a imagem que sera passada como argumento ao executar o programa
  image= imread(argv[1],CV_LOAD_IMAGE_COLOR);

  //checagem se a imagem abriu ou nao
  if(!image.data){
    cout << "nao abriu" << argv[1] << endl;
    cout << argv[0] << " imagem.jpg";
    exit(0);
  }

  //Aplicação do algoritimo pontilhismo feito pelo professor e reutilizado neste
  srand(time(0));

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

  //agora aproveitaremos o algoritimo de canny para melhorar o pontilhismo
  //primeiro aplicamos um threshold baixo para plotar o maior numero de bordas
  threshold = 50;
  Canny(image, border, threshold, 3*threshold);

  //abaixamos o raio para quatro
  RAIO = 4;

  //vasculhamos a imagem resultante do algoritimo de canny reassentuando os pontos de borda, plotando os circulos nesses pontos agora c raio 4
  for(i=0;i<height;i++){
    for(j=0;j<width;j++){
      //checa se o pixel atual não é de fundo, ou seja, preto
      if(border.at<uchar>(i,j)>0){
        //se nao for ele pega os valor da cor do pixel em RGB
        r = image.at<cv::Vec3b>(i,j)[2];
        g = image.at<cv::Vec3b>(i,j)[1];
        b = image.at<cv::Vec3b>(i,j)[0];
        //Coloca um circulo na mesma posição do pixel de borda na imagem resultante com as cores RGB coletadas
        circle(points,
             cv::Point(j,i),
             RAIO,
             CV_RGB(r,g,b),
             -1,
             CV_AA);
      }
    }
  }
  
  //aumentamos mais o theshold para plotar agora menos bordas, porém essas bordas sao mais acentuadas que a anterior
  //reaplicamos o canny
  threshold = 100;
  Canny(image, border, threshold, 3*threshold);
  
  //por serem mais fortes esses novos pontos vamos diminuir mais o raio do circulo
  RAIO = 3;

  for(i=0;i<height;i++){
    for(j=0;j<width;j++){
      if(border.at<uchar>(i,j)>0){
        r = image.at<cv::Vec3b>(i,j)[2];
        g = image.at<cv::Vec3b>(i,j)[1];
        b = image.at<cv::Vec3b>(i,j)[0];
        circle(points,
             cv::Point(j,i),
             RAIO,
             CV_RGB(r,g,b),
             -1,
             CV_AA);
      }
    }
  }

  //fazemos de novo o processo anterior para um threshold de 150 e com raio 2
  threshold = 150;
  Canny(image, border, threshold, 3*threshold);

  RAIO = 2;

  for(i=0;i<height;i++){
    for(j=0;j<width;j++){
      if(border.at<uchar>(i,j)>0){
        r = image.at<cv::Vec3b>(i,j)[2];
        g = image.at<cv::Vec3b>(i,j)[1];
        b = image.at<cv::Vec3b>(i,j)[0];
        circle(points,
             cv::Point(j,i),
             RAIO,
             CV_RGB(r,g,b),
             -1,
             CV_AA);
      }
    }
  }

  //E por fim, as bordas mais acentuadas, com threshold de 200, usamos um raio = 1
  threshold = 200;
  Canny(image, border, threshold, 3*threshold);

  RAIO = 1;

  for(i=0;i<height;i++){
    for(j=0;j<width;j++){
      if(border.at<uchar>(i,j)>0){
        r = image.at<cv::Vec3b>(i,j)[2];
        g = image.at<cv::Vec3b>(i,j)[1];
        b = image.at<cv::Vec3b>(i,j)[0];
        circle(points,
             cv::Point(j,i),
             RAIO,
             CV_RGB(r,g,b),
             -1,
             CV_AA);
      }
    }
  }

  //após o processo mostramos a imagem resultado
  imshow("Saida", points);
  //espera qualquer tecla a ser precionada
  waitKey();
  //escreve um arquivo de saida da imagem resultado denominada "cannypontos.png"
  imwrite("cannypoints.png", points);
  return 0;
} 

