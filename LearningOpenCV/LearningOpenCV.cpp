// LearningOpenCV.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <opencv2\opencv.hpp>

using namespace cv;
int _tmain(int argc, _TCHAR* argv[])
{
	Mat srcImage = imread("E:\\openCV document\\picture\\1.jpg");
	namedWindow("Example", CV_WINDOW_AUTOSIZE);
	imshow("Example", srcImage);
	waitKey(0);
	return 0;
}

