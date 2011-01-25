#include "stdafx.h"

namespace zz
{
	void SplitChannel3_1(cv::Mat &s3,cv::Mat &d1,cv::Mat &d2,cv::Mat &d3)
	{
		cv::Mat out[]={d1,d2,d3};
		int channelmapping[]={0,0,1,1,2,2};
		cv::mixChannels(&s3,3,out,3,channelmapping,3);
	}
	void BitwiseAnd3(cv::Mat &s1,cv::Mat &s2,cv::Mat &s3,cv::Mat &d1)
	{
		cv::bitwise_and(s1,s2,d1,s1);
		cv::bitwise_and(d1,s3,d1,d1);
	}

	void zColor(cv::Mat &frame,cv::Mat &dst,int hmin,int hmax,int smin,int smax,int vmin,int vmax,int erode,int dilate)
	{
		cv::Mat hsv;
		cv::Mat hue,sat,val;

		int CAM_ROWS=frame.rows,
			CAM_COLS=frame.cols;

		hue=cv::Mat::zeros(CAM_ROWS,CAM_COLS,CV_8UC1);
		sat=cv::Mat::zeros(CAM_ROWS,CAM_COLS,CV_8UC1);
		val=cv::Mat::zeros(CAM_ROWS,CAM_COLS,CV_8UC1);
		dst=cv::Mat::zeros(CAM_ROWS,CAM_COLS,CV_8UC1);

		cv::cvtColor(frame,hsv,CV_BGR2HSV);

		SplitChannel3_1(hsv,hue,sat,val);

		//threshold
		cv::inRange(hue,hmin,hmax,hue);
		cv::inRange(sat,smin,smax,sat);
		cv::inRange(val,vmin,vmax,val);
		//end threshold

		//and
		dst=cv::Mat::zeros(CAM_ROWS,CAM_COLS,CV_8UC1);
		BitwiseAnd3(hue,sat,val,dst);
		//end and

		//erode&dilate
		cv::erode(dst,dst,cv::getStructuringElement(cv::MORPH_RECT,cv::Size(3,3)),cv::Point(-1,-1),erode);
		cv::dilate(dst,dst,cv::getStructuringElement(cv::MORPH_RECT,cv::Size(5,5)),cv::Point(-1,-1),dilate);
		//end erode&dilate

	}
}