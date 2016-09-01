// LearningOpenCV.cpp : 定义控制台应用程序的入口点。
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

