// OpenCVTest2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;
using namespace std;

double maxVal = 0;
double minVal = 0;
int e_slider;
int e_slidermax;
//Mat dstM;
void on_trackbar(int, void*) {

}

int main(int argc, const char** argv)
{
	namedWindow("original", 1);
	namedWindow("threshold", 1);
	VideoCapture cap("test.mp4");
	Mat frame;
	Mat dst;
	//trackbar
	e_slider = 0;
	e_slidermax = 50;
	createTrackbar("Epsilon", "threshold", &e_slider, e_slidermax, on_trackbar);
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


		//показываем кадр
		imshow("original", frame);
		imshow("threshold", dst);
		char c = cvWaitKey(33);
		if (c == 27) { 
			break;
		}
	}

	return 0;
}