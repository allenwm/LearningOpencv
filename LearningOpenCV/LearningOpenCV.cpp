// LearningOpenCV.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2\opencv.hpp>

using namespace cv;
int _tmain(int argc, _TCHAR* argv[])
{
	VideoCapture capture("E:\\Download\\EP17.mp4");

	bool suspend = false;
	char key;
	while (1)
	{
		Mat frame;
		capture >> frame;
		imshow("Load a AVI", frame);

		key = waitKey(30);
		if (key == 'q')
		{
			suspend = true;
		}
		while (suspend)
		{
			key = waitKey(1);
			if (key == 'q')
			{
				suspend = false;
			}
		}
		if (key == 27)
		{
			break;
		}
	}
	return 0;
}

