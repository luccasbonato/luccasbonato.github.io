#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char**){
  Mat image;
  int p1x, p1y, p2x, p2y;
	

  image= imread("biel.png",CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data)
    cout << "nao abriu biel.png" << endl;

  namedWindow("janela",WINDOW_AUTOSIZE);

  cout << "Digite o valor de P1:\nValor X: ";
  cin >> p1x;
  cout << "Valor Y: ";
  cin >> p1y;
  cout << "Digite o valor de P2:\nValor X: ";
  cin >> p2x;
  cout << "Valor Y: ";
  cin >> p2y;
  
  //Negativando a imagem na regiao selecionada
  for(int i=p1x;i<p2x;i++){
    for(int j=p1y;j<p2y;j++){
      image.at<uchar>(i,j)=255-image.at<uchar>(i,j);
    }
  }

  imshow("janela", image);
  imwrite("negativo.png", image);  
  waitKey();
  return 0;
}
