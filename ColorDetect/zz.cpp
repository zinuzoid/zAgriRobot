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
	unsigned char *zReadNPK()
	{
		unsigned char fbuff[2];
		std::FILE *Fnpk=std::fopen("npk.txt","r");
		std::fread(fbuff,1,2,Fnpk);
		std::fclose(Fnpk);
		return fbuff;
	}
	void zDecision(int soilleft,int soilright,int spotup,int spotdown,int *soilpercentout)
	{
		unsigned char op_send,op_l,op_r;
		int	soilpercent=50;
		if((soilleft+soilright)>20000)soilpercent=soilleft*100/(soilleft+soilright);

		if(spotdown>4000)
		{
			unsigned char op_npk,*snpk;
			snpk=zReadNPK();

			op_send=0x3F;//0b00111111//tell
			op_npk=(snpk[0]-'0');
			op_npk=op_npk|((snpk[1]-'0')<<4);

			std::printf("%3d%3d#%c,%c",op_send,op_npk,snpk[0],snpk[1]);

			std::ostringstream sbuff;
			sbuff<< op_send;
			zz::zSerial::SerialWrite(sbuff.str().data());
			sbuff<< op_npk;
			zz::zSerial::SerialWrite(sbuff.str().data());

			//exit(0);//////////////////
		}
		else 
		{
			if(soilleft+soilright>20000)
			{
				soilpercent=soilleft*100/(soilleft+soilright);
				if(soilpercent<30)
				{
					op_l=0;
					op_r=7;
				}
				else if(soilpercent>70)
				{
					op_l=7;
					op_r=0;
				}
				else
				{
					op_l=7;
					op_r=7;
				}
			}
			else
			{
				op_l=7;
				op_r=7;
			}
			op_send=0x80;//0b10000000
			op_send|=op_l<<3;
			op_send|=op_r;

			std::printf("%3d#",op_send);

			std::ostringstream sbuff;
			sbuff<< op_send;
			zz::zSerial::SerialWrite(sbuff.str().data());
		}

		*soilpercentout=soilpercent;
	}
}

