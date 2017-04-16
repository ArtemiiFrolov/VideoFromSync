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

Mat frame;
Mat dst;
Mat canny_output;
vector<vector<Point> > contours;
vector<Vec4i> hierarchy;

RNG rng(12345);

void on_trackbar(int, void*) {

}

int main(int argc, const char** argv)
{
	//namedWindow("original", 1);
	//namedWindow("threshold", 1);
	namedWindow("output",1);
	VideoCapture cap("test.mp4");
	
	//trackbar
	e_slider = 0;
	e_slidermax = 50;
//	createTrackbar("Epsilon", "threshold", &e_slider, e_slidermax, on_trackbar);
	createTrackbar("Epsilon", "output", &e_slider, e_slidermax, on_trackbar);
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
		//Canny(dst, canny_output, 100, 200, 3);
		



		//В одном окне
		Size s = frame.size();
		Mat output(s.height, s.width * 2, frame.type());
		Mat help1(output, Rect(0, 0, s.width, s.height));
		Mat help2(output, Rect(s.width, 0, s.width, s.height));
		frame.copyTo(help1);
		dst.copyTo(help2);





		//показываем кадр
		//imshow("original", frame);
		//imshow("threshold", dst);
		imshow("output", output);
		char c = cvWaitKey(33);
		if (c == 27) { 
			break;
		}
	}

	return 0;
}