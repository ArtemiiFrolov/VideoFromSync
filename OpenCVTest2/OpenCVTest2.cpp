// OpenCVTest2.cpp: определ€ет точку входа дл€ консольного приложени€.
//

#include "stdafx.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/core/core.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;

const int epsOver = 250;
const int epsMax = 1;

double maxVal = 0;
double minVal = 0;
int e_slider, e_slidermax, x_slider, x_slidermax, y_slider, y_slidermax, a_slider, a_slidermax;
int savedX = 0;
int savedY = 0;
int countMax = 0;
int avgMax = 0;
int frameCounter=0;
int countOver = 0;
int avgOver = 0;


Mat frame;
Mat dst;
Mat dst2;
Mat graphic;
//vector<vector<Point>> contours;
//vector<Vec4i> hierarchy;


void on_trackbar(int, void*) {

}

int main(int argc, const char** argv)
{
	//namedWindow("original", 1);
	//namedWindow("threshold", 1);
	namedWindow("output",1);
	//VideoCapture cap("Test2.mp4");
	VideoCapture cap("rtsp://192.168.5.4/live.sdp");
	cap >> frame;
	//trackbar
	e_slider = 100;
	e_slidermax = 255;
	x_slider = 508;
	x_slidermax = frame.cols;
	y_slider = 327;
	y_slidermax = frame.rows;
	a_slider = 63;
	a_slidermax = 100;

	
	createTrackbar("X", "output", &x_slider, x_slidermax, on_trackbar);
	createTrackbar("Y", "output", &y_slider, y_slidermax, on_trackbar);
	createTrackbar("a", "output", &a_slider, a_slidermax, on_trackbar);
	createTrackbar("Epsilon", "output", &e_slider, e_slidermax, on_trackbar);
	while (cap.isOpened())
	{
		cap >> frame;
		if (frame.empty()) 
		{
			break;
		}
		//cvtColor(frame, dst, COLOR_RGB2GRAY);
		frameCounter++;
		dst = frame.clone();
		int x2 = x_slider + a_slider;
		int y2 = y_slider + a_slider;
		if (x2 >= dst.cols) {
			a_slider = dst.cols - x_slider;
		}
		if (y2 >= dst.rows) {
			a_slider = dst.rows- y_slider;
		}
		dst = dst(Rect(x_slider, y_slider, a_slider, a_slider));
		
		rectangle(frame, Point(x_slider, y_slider), Point(x2, y2), Scalar(0, 55, 255), +1, 8);
		if (a_slider > 0) {


			
			//smooth
			GaussianBlur(dst, dst, Size(3, 3), 0, 0);
			//findmax
			minMaxLoc(dst, &minVal, &maxVal);
			threshold(dst, dst, maxVal - e_slider, maxVal, 3);
			int *avgRow = new int[a_slider];
			int firstRow = 0;
			int lastRow = 0;
			int maxRow = 0;
			int maxRow2 = 0;
			int maxRowI = 0;
			int maxRowI2 = 0;
			
			bool flagFirst = true;
			bool flagLast = false;
			cvtColor(dst, dst2, COLOR_RGB2GRAY);
			for (int i = 0; i < dst.rows; i++)
			{
				int avgX = 0;
				int avgN = 0;
				for (int j = 0; j < dst.cols; j++)
				{
					Scalar intensity = dst2.at<uchar>(i, j);
					if (intensity.val[0] > 0) {
						avgX += intensity.val[0];
						if (intensity.val[0]+epsMax>= maxVal){
							Vec3b & color = dst.at<Vec3b>(i, j);
							color[1] = 255;
							color[2] = 255;
							color[0] = 0;
							countMax++;
						}
						if (intensity.val[0] > epsOver){
							countOver++;
						}
						avgN++;
					}
				}
				if (avgN != 0)
				{
					if (flagFirst)
					{
						firstRow = i;
						flagFirst = false;
					}
					avgRow[i] = avgX / avgN;
					if (avgRow[i] > maxRow)
					{
						maxRow = avgRow[i];
						maxRowI = i;
					}
					if (avgRow[i] == maxRow)
					{	
						maxRow2 = maxRow;
						maxRowI2 = i;
					}
					flagLast = true;
					
				}
				else {
					avgRow[i] = 0;
					if (flagLast) 
					{
						lastRow = i - 1;
						flagLast = false;
					}
				}
			}
			if (flagLast)
			{
				lastRow = a_slider;
			}
			graphic = Mat::zeros(dst.rows, 256, CV_8UC3);
			for (int i = 0; i < dst.rows; i++) {
				line(graphic, Point(0, i), Point(avgRow[i], i), Scalar(0, 55, 255), 1, 8, 0);
			}
			if (maxRow == maxRow2){
				maxRowI = (maxRowI + maxRowI2) / 2;
			} 
			line(graphic, Point(0, (lastRow + firstRow) / 2), Point(255, (lastRow + firstRow) / 2), Scalar(55, 255, 0), 1, 8, 0);
			line(graphic, Point(0, maxRowI), Point(255, maxRowI), Scalar(255, 55, 0), 1, 8, 0);
			line(graphic, Point(epsOver, 0), Point(epsOver, a_slider), Scalar(0, 255, 255), 1, 8, 0);
			delete[]avgRow;
		}


		

		//¬ одном окне
		Size s = frame.size();
		Size s1 = dst.size();
		Size s2 = graphic.size();
	//	Mat output(s.height+s1.height, max(s.width,s1.width+s2.width), frame.type());
		/*Mat help1(output, Rect(0, 0, s.width, s.height));
		Mat help2(output, Rect(0, s.height, s1.width, s1.height));
		Mat help3(output, Rect(a_slider, s.height, s2.width, s1.height));*/
		Mat output(s.height, s.width+s1.width + s2.width, frame.type());
		Mat help1(output, Rect(0, 0, s.width, s.height));
		Mat help2(output, Rect(s.width, 0, s1.width, s1.height));
		Mat help3(output, Rect(s.width+a_slider, 0, s2.width, s1.height));

		frame.copyTo(help1);
		dst.copyTo(help2);
		graphic.copyTo(help3);
		
		if (frameCounter == 30){
			frameCounter = 0;
			avgMax = countMax / 30;
			countMax = 0;
			avgOver = countOver / 30;
			countOver = 0;			
		}
		putText(output, to_string(avgMax), cvPoint(s.width+10, a_slider+10),	FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200, 200, 250), 1, CV_AA);
		putText(output, to_string(avgOver), cvPoint(s.width + 10, a_slider + 40), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0, 250, 250), 1, CV_AA);

		//показываем кадр
		//imshow("original", frame);
		//imshow("threshold", canny_output);
		imshow("output", output);
		
		char c = cvWaitKey(33);
		if (c == 27) { 
			break;
		}
	}

	return 0;
}