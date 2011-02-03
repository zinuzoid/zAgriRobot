/*------------------------------------------------------------------------------------------------
** Program 	:	zz.hpp
** Project	:	Realtime
** CPU		:	STM32F100
** Date		:	3/2/2011
** Modify	:	Jetsada Machom <Zinuzoid Corporation Co.,Ltd.> zinuzoid@zinuzoid.com
** Copyright:	
** Note 	:	
------------------------------------------------------------------------------------------------*/
#include "stdafx.h"

#define ZCOMPORT	"COM1"

#define ZPIXELTHRESHOLD		20000
#define ZSPOTTHRESHOLD		4000
#define ZPERCENTTHRESHOLD	5

namespace zz
{
	namespace zSerial
	{
		bool SerialInit();
		void SerialClose();
		void SerialWrite(const char *str);
	}
	namespace zSetting
	{
		void SaveSetting(const char *filename,
			int soilhmin,int soilhmax,int soilsmin,int ssoilmax,int soilvmin,int soilvmax,int soilerode,int soildilate,
			int spothmin,int spothmax,int spotsmin,int sspotmax,int spotvmin,int spotvmax,int spoterode,int spotdilate);
		void LoadSetting(const char *filename,
			int *soilhmin,int *soilhmax,int *soilsmin,int *soilsmax,int *soilvmin,int *soilvmax,int *soilerode,int *soildilate,
			int *spothmin,int *spothmax,int *spotsmin,int *spotsmax,int *spotvmin,int *spotvmax,int *spoterode,int *spotdilate);
	}
	void zDetectColor(cv::Mat &frame,cv::Mat &dst,int hmin,int hmax,int smin,int smax,int vmin,int vmax,int erode,int dilate);
	void zScanPixel(cv::Mat &frame,cv::Rect box,int *pixel);
	void zDecision(int soilleft,int soilright,int spotup,int spotdown,int *soilpercentout); 
	//void SplitChannel3_1(cv::Mat &s3,cv::Mat &d1,cv::Mat &d2,cv::Mat &d3);
	//void BitwiseAnd3(cv::Mat &s1,cv::Mat &s2,cv::Mat &s3,cv::Mat &d1);
}
