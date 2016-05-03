#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int mask, char** argv){
  Mat image;
  
  image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data)
    cout << "nao foi possivel abrir a imagem " << endl;
  
  int width  = image.size().width;
  int height = image.size().height;
 
  Mat imageFinal (height,width,DataType<uchar>::type);
  Mat A = image(Rect(0, 0, height/2, width/2));
  Mat B = image(Rect(128,0,height/2, width/2));
  Mat C = image(Rect(0, 128, height/2, width/2));
  Mat D = image(Rect(128,128,height/2, width/2));
      
  A.copyTo(imageFinal(Rect(128,128,height/2,width/2)));
  B.copyTo(imageFinal(Rect(0,128,height/2,width/2)));
  C.copyTo(imageFinal(Rect(128,0,height/2,width/2)));
  D.copyTo(imageFinal(Rect(0,0,height/2,width/2)));

  imshow("janelaFinal", imageFinal);  
  waitKey();
  return 0;
}
