#ifndef _Saliency_H_
#define _Saliency_H_
#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

class CSaliency
{
public:
	CSaliency();
	~CSaliency();
	void GetSalMap(Mat mSrcImg, Mat& mDstImg);

private:
	void _RGB2Lab(Mat &	m_SrcImg, vector<Mat> & vlabImg);
	void _GaussianSmooth(Mat& singleImg);

};
#endif
