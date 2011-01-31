#include "stdafx.h"

namespace zz
{
	void zDetectColor(cv::Mat &frame,cv::Mat &dst,int hmin,int hmax,int smin,int smax,int vmin,int vmax,int erode,int dilate);
	void zScanPixel(cv::Mat &frame,cv::Rect box,int *pixel);
	void zDecision(int soilleft,int soilright,int spotup,int spotdown,int *soilpercentout);
	//void SplitChannel3_1(cv::Mat &s3,cv::Mat &d1,cv::Mat &d2,cv::Mat &d3);
	//void BitwiseAnd3(cv::Mat &s1,cv::Mat &s2,cv::Mat &s3,cv::Mat &d1);
	namespace zSerial
	{
		void SerialInit();
		void SerialClose();
		void SerialWrite(const char *str);
	}
}
