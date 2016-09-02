// LearningOpenCV.cpp : �������̨Ӧ�ó������ڵ㡣
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

//------------------------��on_Trackbar()������------------------
void on_Trackbar(int, void*)
{
	//�����ǰ����
	g_preFrameNumber = g_totalFrameNumber * (float(g_nScheduleValueSlider / (g_nMaxSchedule * 1.0)));
	capture.set(CV_CAP_PROP_POS_FRAMES, g_preFrameNumber);
}

int _tmain(int argc, _TCHAR* argv[])
{
	capture.open(("E:\\Download\\EP17.mp4"));

	//��ȡ����֡��
	g_totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);
	cout << "������Ƶ��" << g_totalFrameNumber << "֡" << endl;

	//���û�������ֵ
	g_nScheduleValueSlider = 0;

	//��������
	namedWindow(WINDOW_NAME);

	//�ڴ����Ĵ����д���һ���������ؼ�
	char TrackbarNmae[500];
	sprintf(TrackbarNmae, "����ֵ��%d %", g_nMaxSchedule);

	createTrackbar(TrackbarNmae, WINDOW_NAME, &g_nScheduleValueSlider, g_nMaxSchedule, on_Trackbar);

	//����ڻص���������ʾ
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

