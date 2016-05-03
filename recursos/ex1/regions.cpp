#include <iostream>
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image;
  Vec3b val;
  int xP1,yP1,xP2,yP2;
  cout << "Insira o ponto P1(x):";
  cin >> xP1;
  cout << "Insira o ponto P1(y):";
  cin >> yP1;
  cout << "Insira o ponto P2(x):";
  cin >> xP2;
  cout << "Insira o ponto P2(x):";
  cin >> yP2;

  image= imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data)
    cout << "nao foi possivel abrir a imagem" << endl;

  namedWindow("janela",WINDOW_AUTOSIZE);

  for(int i=xP1;i<xP2;i++){
    for(int j=yP1;j<yP2;j++){
      image.at<uchar>(i,j) = image.at<uchar>(i,j)*(-1);
    }
  }
  
  imshow("janela", image);  
  waitKey();
  return 0;
}
