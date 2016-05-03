#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv){
  Mat image, mask;
  int width, height;
  int qtnTotalObjetos = 0;
  int qtnComBuraco = 0;
  int possuiBuraco = 100;
  int naoPossuiBuraco = 200;
  
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

  // Cria uma margem de 1 pixel branca
  for (int x = 0; x < height; x++)
  {
  	for (int y = 0; y < width; y++)
  	{
  	  if(x == 0 || x == height-1)
  	  {
  	    image.at<uchar>(x, y) = 255;
  	  }
  	  if(y == 0 || y == width-1)
  	  {
  	    image.at<uchar>(x, y) = 255;
  	  }
  	}
  }

  // Aplica floodfill no ponto inicial para tirar todas
  // as bolhas que tocam a margem e ela mesma
  floodFill(image, p, 0);

  //Aplica floodfill para criar um plano de fundo com 
  // valor diferente dos que estao nos buracos das bolhas
  floodFill(image, p, 1);

  // Marca todos os objetos como sem bolha
  for(int i=0; i<height; i++)
  {
    for(int j=0; j<width; j++)
    {
      if(image.at<uchar>(i,j) == 255)
      {
		    // achou um objeto
		    qtnTotalObjetos++;
		    p.x=j;
		    p.y=i;
		    floodFill(image, p, naoPossuiBuraco);
	    }
	  }
  }

  // Fazer uma busca dos objetos com buracos e marcar 
  //como possuiBuraco 
  for (int x = 0; x < height; x++) 
  {
    for (int y = 0; y < width; y++) 
    {
      if(image.at<uchar>(x,y) == naoPossuiBuraco || image.at<uchar>(x,y) == possuiBuraco) 
      {
        p.x = y;
        p.y = x;
      } 
      else if (image.at<uchar>(x,y) == 0) 
      {
        if (image.at<uchar>(p.y, p.x) == naoPossuiBuraco) 
        {
          floodFill(image, p, possuiBuraco);
          qtnComBuraco++;
        }
        p.x = y;
        p.y = x;
        floodFill(image, p, possuiBuraco);
      }
    }
  }

  imshow("image", image);
  imwrite("labeling.png", image);
  printf("Quantidade de elementos sem bolhas: %d\n", qtnTotalObjetos - qtnComBuraco);
  printf("Quantidade de elementos com bolhas: %d\n", qtnComBuraco);
  waitKey();
  return 0;
}

