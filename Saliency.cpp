#include "stdafx.h"
#include "Saliency.h"


CSaliency::CSaliency()
{
}


CSaliency::~CSaliency()
{
}

void CSaliency::_RGB2Lab(Mat & m_SrcImg, vector<Mat> & vlabImg)
{
	for (int i = 0; i < vlabImg.size(); i++)
	{
		vlabImg[i] = Mat(m_SrcImg.size(), CV_64FC1, Scalar::all(0));
	}
	for (int r = 0; r < m_SrcImg.rows; r++)
	{
		Vec3b *pVec3bSrc = m_SrcImg.ptr<Vec3b>(r);
		double *pVlabImgl = vlabImg[0].ptr<double>(r);
		double *pVlabImga = vlabImg[1].ptr<double>(r);
		double *pVlabImgb = vlabImg[2].ptr<double>(r);

		for (int c = 0; c<m_SrcImg.cols; c++)
		{
			int sR = (int)pVec3bSrc[c][0];
			int sG = (int)pVec3bSrc[c][1];
			int sB = (int)pVec3bSrc[c][2];
			//------------------------
			// sRGB to XYZ conversion
			// (D65 illuminant assumption)
			//------------------------
			double R = (double)sR / 255.0;
			double G = (double)sG / 255.0;
			double B = (double)sB / 255.0;

			double r, g, b;

			if (R <= 0.04045)	r = (double)R / 12.92;
			else				r = pow((R + 0.055) / 1.055, 2.4);
			if (G <= 0.04045)	g = (double)G / 12.92;
			else				g = pow((G + 0.055) / 1.055, 2.4);
			if (B <= 0.04045)	b = (double)B / 12.92;
			else				b = pow((B + 0.055) / 1.055, 2.4);

			double X = r*0.4124564 + g*0.3575761 + b*0.1804375;
			double Y = r*0.2126729 + g*0.7151522 + b*0.0721750;
			double Z = r*0.0193339 + g*0.1191920 + b*0.9503041;
			//------------------------
			// XYZ to LAB conversion
			//------------------------
			double epsilon = 0.008856;	//actual CIE standard
			double kappa = 903.3;		//actual CIE standard

			double Xr = 0.950456;	//reference white
			double Yr = 1.0;		//reference white
			double Zr = 1.088754;	//reference white

			double xr = X / Xr;
			double yr = Y / Yr;
			double zr = Z / Zr;

			double fx, fy, fz;
			if (xr > epsilon)	fx = pow(xr, 1.0 / 3.0);
			else				fx = (kappa*xr + 16.0) / 116.0;
			if (yr > epsilon)	fy = pow(yr, 1.0 / 3.0);
			else				fy = (kappa*yr + 16.0) / 116.0;
			if (zr > epsilon)	fz = pow(zr, 1.0 / 3.0);
			else				fz = (kappa*zr + 16.0) / 116.0;

			pVlabImgl[c] = 116.0*fy - 16.0;
			pVlabImga[c] = 500.0*(fx - fy);
			pVlabImgb[c] = 200.0*(fy - fz);
		}
	}
}

void CSaliency::_GaussianSmooth(Mat& singleImg)
{
	vector<double> kernel(0);
	kernel.push_back(1.0);
	kernel.push_back(4.0);
	kernel.push_back(6.0);
	kernel.push_back(4.0);
	kernel.push_back(1.0);

	int center = int(kernel.size()) / 2;

	Mat tempImg(singleImg.size(), CV_64FC1);
	//--------------------------------------------------------------------------
	// Blur in the x direction.
	//---------------------------------------------------------------------------
	for (int r = 0; r < singleImg.rows; r++)
	{
		double* pSingleImg = singleImg.ptr<double>(r);
		double* pTempImg = tempImg.ptr<double>(r);
		for (int c = 0; c < singleImg.cols; c++)
		{
			double kernelsum(0);
			double sum(0);
			for (int cc = (-center); cc <= center; cc++)
			{
				if (((c + cc) >= 0) && ((c + cc) < singleImg.cols))
				{
					sum += pSingleImg[c + cc] * kernel[center + cc];
					kernelsum += kernel[center + cc];
				}
			}
			pTempImg[c] = sum / kernelsum;
		}
	}

	//--------------------------------------------------------------------------
	// Blur in the y direction.
	//---------------------------------------------------------------------------
	//	Mat smoothImg(tempImg.size(),CV_64FC1,Scalar::all(0));
	for (int r = 0; r < tempImg.rows; r++)
	{
		//		double* pSmoothImg = smoothImg.ptr<double>(r);
		double* pSingleImg = singleImg.ptr<double>(r);
		for (int c = 0; c < tempImg.cols; c++)
		{
			double kernelsum(0);
			double sum(0);
			for (int rr = (-center); rr <= center; rr++)
			{
				if (((r + rr) >= 0) && ((r + rr) < tempImg.rows))
				{
					double* pTempImg = tempImg.ptr<double>(r + rr);
					sum += pTempImg[c] * kernel[center + rr];
					kernelsum += kernel[center + rr];
				}
			}
			pSingleImg[c] = sum / kernelsum;
		}
	}
}

void CSaliency::GetSalMap(Mat mSrcImg, Mat& mDstImg)
{
	if (mSrcImg.channels() == 1)
		cvtColor(mSrcImg, mSrcImg, COLOR_GRAY2BGR);

	mDstImg = Mat(mSrcImg.size(), CV_64FC1, Scalar::all(0));

	vector<Mat> vlabImg(mSrcImg.channels());
	vector<double> meanVal(mSrcImg.channels());
	_RGB2Lab(mSrcImg, vlabImg);

	for (int i = 0; i<vlabImg.size(); i++)
	{
		Scalar meanVals = mean(vlabImg[i]);
		meanVal[i] = meanVals.val[0];
	}

	_GaussianSmooth(vlabImg[0]);
	_GaussianSmooth(vlabImg[1]);
	_GaussianSmooth(vlabImg[2]);

	int rowNumber = mDstImg.rows;
	int colNumber = mDstImg.cols;

	for (int i = 0; i < rowNumber; i++)
	{
		double * data = mDstImg.ptr<double>(i);
		double * data0 = vlabImg[0].ptr<double>(i);
		double * data1 = vlabImg[1].ptr<double>(i);
		double * data2 = vlabImg[2].ptr<double>(i);

		for (int j = 0; j < colNumber; j++)
		{
			data[j] = (data0[j] - meanVal[0])*(data0[j] - meanVal[0]) + (data1[j] - meanVal[1])*(data1[j] - meanVal[1]) + (data2[j] - meanVal[2])*(data2[j] - meanVal[2]);
		}
	}
	//¾ùÒ»»¯
	normalize(mDstImg, mDstImg, 1, 0, NORM_MINMAX);
}