#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat imga, histb, hista;
  VideoCapture cap;
  double comp;

  double limiar = 0.99;

  int nbins = 64;
  float range[] = {0, 256};
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;

  vector<Mat> pla;

  cap.open(0);
  
  if(!cap.isOpened()){
    cout << "cameras indisponiveis";
    return -1;
  }
  
  //captura a primeira imagem base
  cap >> imga;

  split( imga, pla );

  calcHist(&pla[1], 1, 0, Mat(), histb, 1, &nbins, &histrange, uniform, acummulate);

  while(1){
    //Captura a imagem nova
    cap >> imga;
    //Converte a imagem para a escala cinza

    //separa as cores RGB
    split( imga, pla );

    //Calcula o histograma de G
    calcHist(&pla[1], 1, 0, Mat(), hista, 1, &nbins, &histrange, uniform, acummulate);

    //Compara o histograma novo com o antigo
    comp = compareHist(hista, histb, CV_COMP_CORREL);
   
    //cout << comp << endl;

    //entra na condição se a comparação for menos que o limiar ele alarma
    if (comp < limiar) {
    putText(imga, 
"Movement!", Point(10, 40), FONT_HERSHEY_TRIPLEX, 1.0, CV_RGB(255,0,0), 2.0);
    }

    imshow("imga", imga);
    
    //Guarda o histograma novo na variável do antigo
    calcHist(&pla[1], 1, 0, Mat(), histb, 1, &nbins, &histrange, uniform, acummulate);
    
    if(waitKey(30) >= 0) break;
  }

  return 0;
}
