#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace cv;
using namespace std;

double ARC;
double FD;
double PVC;
double alpha;

int sldrARC;
int sldrARCmax;
int sldrFD;
int sldrFDmax;
int sldrPVC;
int sldrPVCmax;

Mat img, borrada, tilt;

char TrackbarName[50];

//Função para borrar a ibagen
void borrar(){
  //Criação da mascara de borramento
  float gauss[] = {1,2,1,
		   2,4,2,
		   1,2,1};
  Mat mask(3,3,CV_32F);
  mask = Mat(3, 3, CV_32F, gauss);
  scaleAdd(mask, 1/16.0, Mat::zeros(3,3,CV_32F), mask);

  img.convertTo(borrada, CV_32F);

  filter2D(borrada, borrada, borrada.depth(), mask, Point(1,1), 0);
  filter2D(borrada, borrada, borrada.depth(), mask, Point(1,1), 0);
  filter2D(borrada, borrada, borrada.depth(), mask, Point(1,1), 0);
  filter2D(borrada, borrada, borrada.depth(), mask, Point(1,1), 0);
  filter2D(borrada, borrada, borrada.depth(), mask, Point(1,1), 0);
  filter2D(borrada, borrada, borrada.depth(), mask, Point(1,1), 0);
  filter2D(borrada, borrada, borrada.depth(), mask, Point(1,1), 0);

  borrada.convertTo(borrada, CV_8U);
}

void saida()
{
  double x_;

  for(int i = 0; i < img.rows; i++){
    //valor de X deslocado
    x_ = i - (PVC + ARC/2.0);
    //calculo do alpha
    alpha = 0.5 * (tanh((x_ + ARC/2)/FD) - tanh((x_ - ARC/2)/FD));
    for(int j = 0; j < img.cols; j++){
      //combinação das ibagens para imagem de saida 
      tilt.at<Vec3b>(i, j) = alpha * img.at<Vec3b>(i, j) + (1-alpha)*borrada.at<Vec3b>(i, j);
    }
  }
}

//aumentar saturação da imagem
void aumentarSat(){
  Mat imghsv, hsv[3];
  cvtColor(tilt, imghsv, COLOR_BGR2HSV);
  split(imghsv, hsv);
  hsv[1] = hsv[1] * 1.8;
  merge(hsv, 3, imghsv);
  cvtColor(imghsv, tilt, COLOR_HSV2BGR);
}

//Alterar o slider da Posição Vertical do Centro
void alterarSldrPVC(int, void*){
  PVC = sldrPVC;
  saida();
  aumentarSat();
  imshow("Tiltshift", tilt);
}

//Alterar slider da Altura da Região central
void alterarSldrARC(int, void*){
  ARC = sldrARC;
  saida();
  aumentarSat();
  imshow("Tiltshift", tilt);
}

//Slider para alterar a Força de Decaimento
void alterarSldrFD(int, void*){
  FD = sldrFD;
  //Para evitar divião por 0 forçamos o minimo igual a 1
  if(FD == 0){
    FD = 1;
  }
  saida();
  aumentarSat();
  imshow("Tiltshift", tilt);
}


int main(int argc, char* argv[]){

//Leitura da imagem
  img = imread(argv[1], CV_LOAD_IMAGE_COLOR);
  
  if (!img.data){
    cout << "Imagem não foi aberta" << endl;
    return -2;
  }

  tilt = img.clone();
  
  //criação da janela e redimensionamento dela
  namedWindow("Tiltshift", WINDOW_NORMAL);
  resizeWindow("Tiltshift", 600,600);

  //Criar imagem borrada
  borrar();

  //inicializar os valores dos sliders
  sldrARC = 1;
  sldrFD = 1;
  sldrPVC = 1;

  //Definir o maximo de cada slider
  sldrARCmax = tilt.rows;
  sldrPVCmax = tilt.rows;
  sldrFDmax = 100;

  //Criação dos sliders
  createTrackbar("Altura", "Tiltshift", &sldrARC, sldrARCmax, alterarSldrARC);
  alterarSldrARC(sldrARC, 0);

  createTrackbar("Decaimento", "Tiltshift", &sldrFD, sldrFDmax, alterarSldrFD );
  alterarSldrFD(sldrFD, 0);

  createTrackbar( "Centro", "Tiltshift", &sldrPVC, sldrPVCmax, alterarSldrPVC );
  alterarSldrPVC(sldrPVC, 0);

  //esperar a tecla esc para finalizar o programa
  while(1){
    if(waitKey(30) >= 0){
      break;
    }
  }

  //Imprimir imagem final
  imwrite("tiltshift.png", tilt);

  return 0;
}
