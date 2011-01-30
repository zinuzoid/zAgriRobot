#include "stdafx.h"
#include <iostream>

namespace zz
{
	namespace zSerial
	{
		DCB dcb;
		HANDLE hPort;
		void SerialInit()
		{
		    hPort=CreateFile(
				_T("COM1"),
				GENERIC_WRITE,
				0,
				NULL,
				OPEN_EXISTING,
				0,
				NULL);

			dcb.BaudRate=CBR_9600;
			dcb.ByteSize=8;
			dcb.Parity=0;
			dcb.StopBits=0;

			if(!GetCommState(hPort,&dcb))
			{
				std::cout<< "err comm";
			}
		}
		void SerialClose()
		{
			CloseHandle(hPort);
		}
		void SerialWrite(const char *str)
		{
			DWORD byteswritten;
			WriteFile(hPort,str,std::strlen(str),&byteswritten,NULL);
		}
	}

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
	void zinRange(cv::Mat &src,uchar a,uchar b,cv::Mat &dst)
	{
		cv::Mat dsttmp=cv::Mat::zeros(src.rows,src.cols,CV_8UC1);
		for(int row=0;row<src.rows;row++)
		{
			uchar *inp=src.ptr<uchar>(row);
			uchar *out=dsttmp.ptr<uchar>(row);
			for(int col=0;col<src.cols;col++)
			{
				if(
					((*inp)>=a)&&
					((*inp)<=b))
				{
					*out=255;
				}
				inp++;
				out++;
			}
		}
		dst=dsttmp;
	}

	void zScanPixel(cv::Mat &frame,cv::Rect box,int *pixel)
	{
		*pixel=0;
		for(int row=box.y;row<box.y+box.height;row++)
		{
			uchar *inp=frame.ptr<uchar>(row);
			inp+=box.x;
			for(int col=0;col<box.width;col++)
			{
				if(*inp!=0)
				{
					(*pixel)++;
				}
				inp++;
			}
		}	
	}
	void zDetectColor(cv::Mat &frame,cv::Mat &dst,int hmin,int hmax,int smin,int smax,int vmin,int vmax,int erode,int dilate)
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
		zz::zinRange(hue,hmin,hmax,hue);
		zz::zinRange(sat,smin,smax,sat);
		zz::zinRange(val,vmin,vmax,val);
		//cv::inRange(hue,hmin,hmax,hue);
		//cv::inRange(sat,smin,smax,sat);
		//cv::inRange(val,vmin,vmax,val);
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
