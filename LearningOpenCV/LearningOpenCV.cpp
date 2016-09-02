// LearningOpenCV.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2\opencv.hpp>
#include <stdio.h>

using namespace cv;
using namespace std;
#define WINDOW_NAME "[test_window]"

//set global variable
const int g_nMaxSchedule = 100;
int g_nScheduleValueSlider;
long g_totalFrameNumber;
long g_preFrameNumber;

VideoCapture capture;

//------------------------【on_Trackbar()函数】------------------
void on_Trackbar(int, void*)
{
	//求出当前进度
	g_preFrameNumber = g_totalFrameNumber * (float(g_nScheduleValueSlider / (g_nMaxSchedule * 1.0)));
	capture.set(CV_CAP_PROP_POS_FRAMES, g_preFrameNumber);
}

int _tmain(int argc, _TCHAR* argv[])
{
	capture.open(("E:\\Download\\EP17.mp4"));

	//获取整个帧数
	g_totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);
	cout << "整个视频共" << g_totalFrameNumber << "帧" << endl;

	//设置滑动条初值
	g_nScheduleValueSlider = 0;

	//创建窗体
	namedWindow(WINDOW_NAME);

	//在创建的窗体中创建一个滑动条控件
	char TrackbarNmae[500];
	sprintf(TrackbarNmae, "进度值：%d %", g_nMaxSchedule);

	createTrackbar(TrackbarNmae, WINDOW_NAME, &g_nScheduleValueSlider, g_nMaxSchedule, on_Trackbar);

	//结果在回调函数中显示
	on_Trackbar(g_nScheduleValueSlider, 0);

	bool suspend = false;
	char key;
	while (1)
	{
		Mat frame;
		capture >> frame;
		g_preFrameNumber++;
		imshow(WINDOW_NAME, frame);

		g_nScheduleValueSlider = 100 * (double(g_preFrameNumber / (g_nMaxSchedule * 1.0)));

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
		if (key == 27 || !frame.data)
		{
			break;
		}
	}
	return 0;
}

