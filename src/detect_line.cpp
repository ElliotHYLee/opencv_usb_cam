#include <ros/ros.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;
class DetectLine
{
  Mat dst, detected_edges;
  int edgeThresh;
  int lowThreshold;
  int max_lowThreshold;
  int ratio;
  int kernel_size;

  int iLowH ;
  int iHighH ;

  int iLowS ;
  int iHighS;

  int iLowV;
  int iHighV ;

public:
  DetectLine()
  {
    edgeThresh = 1;
    lowThreshold = 200;
    max_lowThreshold = 300;
    ratio = 3;
    kernel_size = 3;


    iLowH = 74;
    iHighH = 179;

    iLowS = 80;
    iHighS = 136;

    iLowV = 114;
    iHighV = 255;
  }

  double processImage(Mat imgInput)
  {
    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"
    //Create trackbars in "Control" window
    cvCreateTrackbar("LowH", "Control", &iLowH, 1); //Hue (0 - 179)
    cvCreateTrackbar("HighH", "Control", &iHighH, 179);

    cvCreateTrackbar("LowS", "Control", &iLowS, 1); //Saturation (0 - 255)
    cvCreateTrackbar("HighS", "Control", &iHighS, 255);

    cvCreateTrackbar("LowV", "Control", &iLowV, 1); //Value (0 - 255)
    cvCreateTrackbar("HighV", "Control", &iHighV, 255);

    imshow("OrigImg", imgInput);
    Mat imgHSV;
    cvtColor(imgInput, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

    Mat imgThresholded;
    inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

    //morphological opening (remove small objects from the foreground)
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

    //morphological closing (fill small holes in the foreground)
    dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    //erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    //erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

    rectangle(imgThresholded, Point(0, 0), Point(639, 475), Scalar(255, 255, 255 ), 1, 4 );

    imshow("Thresholded Image", imgThresholded); //show the thresholded image
    imshow("Original", imgInput); //show the original image



    // Mat gray;
    // cvtColor(imgInput, gray, CV_BGR2GRAY);
    // imshow("GrayImg", gray);
    //
    Canny(imgThresholded, detected_edges, lowThreshold, max_lowThreshold, kernel_size);
    imshow("EdgeImg", detected_edges);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    rectangle(detected_edges, Point(0, 0), Point(639, 475), Scalar(255, 255, 255 ), 1, 4 );
    findContours(detected_edges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    Mat drawing = Mat::zeros(detected_edges.size(), CV_8UC3 );
    double counter=0;
    double area = 0, prob = 0;
    for( int i = 0; i< contours.size(); i++ )
    {
      area = contourArea(contours[i]);
      Scalar color =  Scalar(0, 0, 255);
      drawContours(imgInput, contours, i, color, 2, 8, hierarchy, 0, Point() );

      if (area> 10000) counter ++;
    }
    prob = counter/contours.size()*100;
    cout << prob << " " << endl;


    // Show in a window
    namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
    imshow( "Contours", imgInput );

    return prob;

    //Mat invtImg;
    //threshold (detected_edges, invtImg, 70, 255, CV_THRESH_BINARY_INV);

    //imshow("InvtImg", invtImg);
    //
    // dilate(detected_edges, detected_edges, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    // dilate(detected_edges, detected_edges, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    // erode(detected_edges, detected_edges, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    // erode(detected_edges, detected_edges, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    //
    // imshow("InvtImg_morph", detected_edges);

    // erode(detected_edges, detected_edges, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    // erode(detected_edges, detected_edges, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    // erode(detected_edges, detected_edges, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    // dilate(detected_edges, detected_edges, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );


  }

};
