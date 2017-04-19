// OpenCVTest2.cpp: определяет точку входа для консольного приложения.
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

double maxVal = 0;
double minVal = 0;
int e_slider, e_slidermax, x_slider, x_slidermax, y_slider, y_slidermax, a_slider, a_slidermax;
int savedX = 0;
int savedY = 0;

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
	VideoCapture cap("Test2.mp4");
	
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
	for (;;)
	{
		cap >> frame;
		if (frame.empty()) 
		{
			break;
		}
		//cvtColor(frame, dst, COLOR_RGB2GRAY);
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
			graphic = Mat::zeros(dst.rows, 255, CV_8UC3);
			for (int i = 0; i < dst.rows; i++) {
				line(graphic, Point(0, i), Point(avgRow[i] * 2, i), Scalar(0, 55, 255), 1, 8, 0);
			}
			line(graphic, Point(0, (lastRow + firstRow) / 2), Point(255, (lastRow + firstRow) / 2), Scalar(55, 255, 0), 1, 8, 0);
			line(graphic, Point(0, maxRow), Point(255, maxRow), Scalar(255, 55, 0), 1, 8, 0);
			
			delete[]avgRow;
		}


		

		//В одном окне
		Size s = frame.size();
		Size s1 = dst.size();
		Size s2 = graphic.size();
		Mat output(s.height+s1.height, max(s.width,s1.width+s2.width), frame.type());
		Mat help1(output, Rect(0, 0, s.width, s.height));
		Mat help2(output, Rect(0, s.height, s1.width, s1.height));
		Mat help3(output, Rect(a_slider, s.height, s2.width, s1.height));
		frame.copyTo(help1);
		dst.copyTo(help2);
		graphic.copyTo(help3);



		/*

		//размываем изображение (тут чтобы избавиться от шума нужна большая фильтрация)
		GaussianBlur(dst, dst, Size(5, 5),0,0);
		//ищем яркие пиксели
		minMaxLoc(dst, &minVal, &maxVal);
		threshold(dst, dst, maxVal - e_slider, maxVal, 3);
		//Canny
	
		int avgX = 0;
		int avgY = 0;
		int avgN = 0;
		//проверяем выход за рамки
		
		//начальное условие
		if (savedX == 0) {
			for (int i = 0; i < dst.cols; i++)
				for (int j = 0; j < dst.rows; j++)
				{
					Vec3b intensity = dst.at<Vec3b>(j, i);
					if ((intensity.val[0] > 0) || (intensity.val[1] > 0) || (intensity.val[2] > 0)) {
						avgX += j;
						avgY += i;
						avgN++;
					}
				}
		}//поиск внутри прямоугольника
		else {
			if (savedX + a_slider > dst.rows) {
				a_slider = dst.rows - savedX;
			}
			if (savedY + b_slider > dst.cols) {
				b_slider = dst.cols - savedY;
			}
			if (savedX - a_slider <0) {
				a_slider = savedX;
			}
			if (savedY - b_slider <0) {
				b_slider = savedX;
			}
			for (int i = savedY - b_slider; i < savedY + b_slider + 1; i++)
				for (int j = savedX - a_slider; j < savedX + a_slider; j++)
				{
					Vec3b intensity = dst.at<Vec3b>(j, i);
					if ((intensity.val[0] > 0) || (intensity.val[1] > 0) || (intensity.val[2] > 0)) {
						avgX += j;
						avgY += i;
						avgN++;
					}
				}
		}
		//поиск среднего
		if (avgN != 0) 
		{
			avgX = avgX / avgN;
			avgY = avgY / avgN;			
		}
		savedX = avgX;
		savedY = avgY;
		//рисуем прямоугольник вокруг точки
		int a0 = savedX - a_slider;
		int b0 = savedY - b_slider;
		int a1 = savedX + a_slider;
		int b1 = savedY + b_slider;
		rectangle(dst, Point(b0,a0), Point(b1, a1), Scalar(0, 55, 255), +1, 8);
		
		if (savedX>0)
		for (int i = 0; i < 9; i++) {
			Vec3b &intensity = dst.at<Vec3b>(avgX-1+i%3, avgY-1+i/3);
			intensity.val[1] = 55;
			intensity.val[2] = 255;
		}
		char result[100];
		sprintf_s(result,  "X is %d, Y is %d", avgY, avgX);
		putText(dst, result, cvPoint(30, 30),	FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200, 200, 250), 1, CV_AA);
		*/




		





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