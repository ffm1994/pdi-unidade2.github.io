#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
int seuil = 10;

void Compare(Mat f, Mat oldF)
{
  if (oldF.rows == 0)
  return;
  for (int i = 0; i < f.rows; i++)
  {
    for (int j = 0; j < f.cols; j++)
    {
    if (abs(f.at<cv::Vec3b>(i, j)[2] - oldF.at<cv::Vec3b>(i, j)[2]) >seuil &&
        abs(f.at<cv::Vec3b>(i, j)[0] - oldF.at<cv::Vec3b>(i, j)[0]) >seuil&&
        abs(f.at<cv::Vec3b>(i, j)[1] - oldF.at<cv::Vec3b>(i, j)[1]) > seuil)
      {
        f.at<cv::Vec3b>(i, j)[2] = 255;
        f.at<cv::Vec3b>(i, j)[1] = 0;
        f.at<cv::Vec3b>(i, j)[0] = 0;
      }
    }
  }
}  

int main(int argc, char** argv){
  Mat image, old_image, imageTemp;
  int width, height;
  VideoCapture cap;
  vector<Mat> planes;
  Mat histR, histG, histB;
  int nbins = 64;
  float range[] = {0, 256};
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;

  cap.open(0);
  
  if(!cap.isOpened()){
    cout << "cameras indisponiveis";
    return -1;
  }
  
  width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  cout << "largura = " << width << endl;
  cout << "altura  = " << height << endl;

  int histw = nbins, histh = nbins/2;
  Mat histImgR(histh, histw, CV_8UC3, Scalar(0,0,0));
  Mat histImgG(histh, histw, CV_8UC3, Scalar(0,0,0));
  Mat histImgB(histh, histw, CV_8UC3, Scalar(0,0,0));

  while(1){
    cap >> image;
    
    image.copyTo(imageTemp);
    Compare(image, old_image);
    imageTemp.copyTo(old_image);

    split (image, planes);
    calcHist(&planes[0], 1, 0, Mat(), histR, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planes[1], 1, 0, Mat(), histG, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planes[2], 1, 0, Mat(), histB, 1,
             &nbins, &histrange,
             uniform, acummulate);

    normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
    normalize(histG, histB, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
    normalize(histB, histB, 0, histImgR.rows, NORM_MINMAX, -1, Mat());

    histImgR.setTo(Scalar(0));
    histImgG.setTo(Scalar(0));
    histImgB.setTo(Scalar(0));
    
    for(int i=0; i<nbins; i++){
      line(histImgR, Point(i, histh),
           Point(i, cvRound(histR.at<float>(i))),
           Scalar(0, 0, 255), 1, 8, 0);
      line(histImgG, Point(i, histh),
           Point(i, cvRound(histG.at<float>(i))),
           Scalar(0, 255, 0), 1, 8, 0);
      line(histImgB, Point(i, histh),
           Point(i, cvRound(histB.at<float>(i))),
           Scalar(255, 0, 0), 1, 8, 0);
    }
    
    imshow("image", image);
    if(waitKey(30) >= 0) break;
  }
  return 0;
}
