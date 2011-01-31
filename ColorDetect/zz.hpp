#include "stdafx.h"

namespace zz
{
	namespace zSerial
	{
		void SerialInit();
		void SerialClose();
		void SerialWrite(const char *str);
	}
	namespace zSetting
	{
		void SaveSetting(const char *filename,
			int soilhmin,int soilhmax,int soilsmin,int ssoilmax,int soilvmin,int soilvmax,int soilerode,int soildilate,
			int spothmin,int spothmax,int spotsmin,int sspotmax,int spotvmin,int spotvmax,int spoterode,int spotdilate);
		void LoadSetting(const char *filename,
			int *soilhmin,int *soilhmax,int *soilsmin,int *ssoilmax,int *soilvmin,int *soilvmax,int *soilerode,int *soildilate,
			int *spothmin,int *spothmax,int *spotsmin,int *sspotmax,int *spotvmin,int *spotvmax,int *spoterode,int *spotdilate);
	}
	void zDetectColor(cv::Mat &frame,cv::Mat &dst,int hmin,int hmax,int smin,int smax,int vmin,int vmax,int erode,int dilate);
	void zScanPixel(cv::Mat &frame,cv::Rect box,int *pixel);
	//void SplitChannel3_1(cv::Mat &s3,cv::Mat &d1,cv::Mat &d2,cv::Mat &d3);
	//void BitwiseAnd3(cv::Mat &s1,cv::Mat &s2,cv::Mat &s3,cv::Mat &d1);
}
