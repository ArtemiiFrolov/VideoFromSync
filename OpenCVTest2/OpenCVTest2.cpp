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
int e_slider, e_slidermax, a_slider, a_slidermax, b_slider, b_slidermax;
int savedX = 0;
int savedY = 0;

Mat frame;
Mat dst;
Mat canny_output;
//vector<vector<Point>> contours;
//vector<Vec4i> hierarchy;


void on_trackbar(int, void*) {

}

int main(int argc, const char** argv)
{
	//namedWindow("original", 1);
	//namedWindow("threshold", 1);
	namedWindow("output",1);
	VideoCapture cap("test.mp4");
	
	//trackbar
	e_slider = 20;
	e_slidermax = 70;
	a_slider = 30;
	a_slidermax = 100;
	b_slider = 30;
	b_slidermax = 100;

	createTrackbar("Epsilon", "output", &e_slider, e_slidermax, on_trackbar);
	createTrackbar("Width", "output", &a_slider, a_slidermax, on_trackbar);
	createTrackbar("Height", "output", &b_slider, b_slidermax, on_trackbar);
	for (;;)
	{
		cap >> frame;
		if (frame.empty()) 
		{
			break;
		}
		dst = frame.clone();
		//избавляемся от совсем шумного шума
		/*minMaxLoc(dst, &minVal, &maxVal);
		threshold(dst, dst, maxVal-5, maxVal-5, 2);*/
		
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
		//В одном окне
		Size s = frame.size(); 
		Mat output(s.height, s.width * 2, frame.type());
		Mat help1(output, Rect(0, 0, s.width, s.height));
		Mat help2(output, Rect(s.width, 0, s.width, s.height));
		frame.copyTo(help1);
		dst.copyTo(help2);





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