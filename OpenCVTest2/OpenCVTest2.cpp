// OpenCVTest2.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"

#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;
using namespace std;
IplImage* frame = 0;
IplImage* dst = 0;
//IplImage* dst2 = 0;
double maxVal = 0;
double minVal = 0;
int e_slider;
int e_slidermax;
Mat dstM;
void on_trackbar(int, void*) {

}

int main(int argc, const char** argv)
{
	// ��� ����� ������� ������ ����������
	char* filename = argc == 2 ? argv[1] : "test.mp4";

	printf("[i] file: %s\n", filename);
	// ���� ��� ����������� ��������
	cvNamedWindow("original", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("threshold", CV_WINDOW_AUTOSIZE);
	// �������� ���������� � �����-�����
	CvCapture* capture = cvCreateFileCapture(filename);
	//trackbar
	e_slider = 0;
	e_slidermax = 50;
	createTrackbar("Epsilon", "threshold", &e_slider, e_slidermax, on_trackbar);
	while (1) {
		// �������� ��������� ����
		frame = cvQueryFrame(capture);
		if (!frame) {
			break;
		}

		
		dst = cvCloneImage(frame);
		dstM = cvarrToMat(dst);
		//����������� �� ������ ������� ����

		/*minMaxLoc(dstM, &minVal, &maxVal);
		cvThreshold(dst, dst, maxVal-5, maxVal-5, 2);*/

		//��������� ����������� (��� ����� ���������� �� ���� ����� ������� ����������)
		cvSmooth(dst, dst, CV_GAUSSIAN, 5, 5);
		dstM = cvarrToMat(dst);
		minMaxLoc(dstM, &minVal, &maxVal);
		cvThreshold(dst, dst, maxVal-e_slider, maxVal, 3);
	//	Canny
		// ���������� ����
		cvShowImage("original", frame);
		cvShowImage("threshold", dst);
		char c = cvWaitKey(33);
		if (c == 27) { // ���� ������ ESC - �������
			break;
		}
	}

	// ����������� �������
	cvReleaseCapture(&capture);
	// ������� ����
	cvDestroyWindow("original");
	cvDestroyWindow("threshold");
	return 0;
}