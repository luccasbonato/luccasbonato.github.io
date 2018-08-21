#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image, mask;
  int width, height;
  
  CvPoint p;
  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  
  if(!image.data){
    std::cout << "imagem nao carregou corretamente\n";
    return(-1);
  }
  width=image.size().width;
  height=image.size().height;

  p.x=0;
  p.y=0;

//Remover bolhas de borda
  for(int i=0; i<height; i++){
    if(image.at<uchar>(0,i) == 255){
	// achou um objeto
	p.x=0;
	p.y=i;
	floodFill(image,p,0);
    }
  }
  for(int i=0; i<height; i++){
    if(image.at<uchar>(i,width-1) == 255){
	// achou um objeto
	p.x=width-1;
	p.y=i;
	floodFill(image,p,0);
    }
  }
  for(int j=0; j<width; j++){
    if(image.at<uchar>(0,j) == 255){
	// achou um objeto
	p.x=j;
	p.y=0;
	floodFill(image,p,0);
    }
  }
  for(int j=0; j<width; j++){
    if(image.at<uchar>(height-1,j) == 255){
	// achou um objeto
	p.x=j;
	p.y=height-1;
	floodFill(image,p,0);
    }
  }

//troca da cor do fundo
  p.x=0;
  p.y=0;
  floodFill(image,p,128);

//Contagem de bolhas
  int nbolhas = 0;
  int nbf = 0;
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      //Achou uma bolha
      if(image.at<uchar>(i,j) == 255){
        nbolhas++;
        p.x=j;
        p.y=i;
        floodFill(image,p,30);
      }
      //Achou um buraco na bolha e pintou ela de outra cor
      if(image.at<uchar>(i,j) == 0){
        p.x=j;
        p.y=i-1;
        floodFill(image,p,200);
        p.x=j;
        p.y=i;
        floodFill(image,p,200);
      }
    }
  }
  //Contando as bolhas com buraco e pintando elas na cor normal
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 200){
        nbf++;
        p.x=j;
        p.y=i;
        floodFill(image,p,30);
      }
    }
  }

  cout << nbolhas << " bolhas foram encontradas." << endl;
  cout << nbf << " bolhas com furos foram encontradas." << endl;

  imshow("image", image);
  imwrite("contar.png", image);
  waitKey();
  return 0;
}
