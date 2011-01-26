#include "stdafx.h"

#include "zz.hpp"

#include <iostream>

cv::Point select_origin;
cv::Rect select_rect;
bool select_on=false;
int
	soilhmin=175,
	soilhmax=185,
	soilsmin=65,
	soilsmax=108,
	soilvmin=136,
	soilvmax=253,
	soilerode=1,
	soildilate=10,

	spothmin=175,
	spothmax=185,
	spotsmin=65,
	spotsmax=108,
	spotvmin=136,
	spotvmax=253,
	spoterode=1,
	spotdilate=10;

void WindowInit()
{
	cv::namedWindow("Raw",cv::WINDOW_AUTOSIZE);
	cv::namedWindow("Soil",cv::WINDOW_AUTOSIZE);
	cv::namedWindow("SoilSetting1",cv::WINDOW_AUTOSIZE);
	cv::namedWindow("SoilSetting2",cv::WINDOW_AUTOSIZE);
	cv::namedWindow("Spot",cv::WINDOW_AUTOSIZE);
	cv::namedWindow("SpotSetting1",cv::WINDOW_AUTOSIZE);
	cv::namedWindow("SpotSetting2",cv::WINDOW_AUTOSIZE);
	cv::createTrackbar("SoilHmin","SoilSetting1",&soilhmin,255,0);
	cv::createTrackbar("SoilHmax","SoilSetting1",&soilhmax,255,0);
	cv::createTrackbar("SoilSmin","SoilSetting1",&soilsmin,255,0);
	cv::createTrackbar("SoilSmax","SoilSetting1",&soilsmax,255,0);
	cv::createTrackbar("SoilVmin","SoilSetting1",&soilvmin,255,0);
	cv::createTrackbar("SoilVmax","SoilSetting1",&soilvmax,255,0);
	cv::createTrackbar("SoilErode","SoilSetting2",&soilerode,20,0);
	cv::createTrackbar("SoilDilate","SoilSetting2",&soildilate,20,0);
	cv::createTrackbar("SpotHmin","SpotSetting1",&spothmin,255,0);
	cv::createTrackbar("SpotHmax","SpotSetting1",&spothmax,255,0);
	cv::createTrackbar("SpotSmin","SpotSetting1",&spotsmin,255,0);
	cv::createTrackbar("SpotSmax","SpotSetting1",&spotsmax,255,0);
	cv::createTrackbar("SpotVmin","SpotSetting1",&spotvmin,255,0);
	cv::createTrackbar("SpotVmax","SpotSetting1",&spotvmax,255,0);
	cv::createTrackbar("SpotErode","SpotSetting2",&spoterode,20,0);
	cv::createTrackbar("SpotDilate","SpotSetting2",&spotdilate,20,0);
}
void WindowRelease()
{
	cv::destroyWindow("Raw");
	cv::destroyWindow("Soil");
	cv::destroyWindow("SoilSetting1");
	cv::destroyWindow("SoilSetting2");
	cv::destroyWindow("Spot");
	cv::destroyWindow("SpotSetting1");
	cv::destroyWindow("SpotSetting2");
}

int _tmain(int argc, _TCHAR* argv[])
{
	cv::Mat frame,soil,spot;
	cv::VideoCapture cap;

	std::cout << "Initial Webcam...";
	cap.open(0);
	if(!cap.isOpened())
	{
		std::printf("err init webcam!");
		std::getchar();
		return 0;
	}
	std::cout << "OK!\n";

	std::cout << "Initial Serial...";
	zz::zSerial::SerialInit();
	std::cout << "OK!\n";

	std::cout << "Initial Window...";
	WindowInit();
	std::cout << "OK!\n";

	cv::waitKey(1000);

	int keycode=-1;
	while(keycode!=27)
	{
		double tt=(double)cv::getTickCount();

		cap >> frame;

		if(frame.empty())
		{
			std::cout<<"err!!!";
			cv::waitKey(0);
			break;
		}

		zz::zDetectColor(frame,soil,soilhmin,soilhmax,soilsmin,soilsmax,soilvmin,soilvmax,soilerode,soildilate);
		zz::zDetectColor(frame,spot,spothmin,spothmax,spotsmin,spotsmax,spotvmin,spotvmax,spoterode,spotdilate);

		int soilleft,soilright,spotup,spotdown;
		const double spotpos=0.8;
		cv::Rect soilleftrect=cv::Rect(0,0,soil.cols/2,soil.rows);
		cv::Rect soilrightrect=cv::Rect(soil.cols/2,0,soil.cols/2,soil.rows);
		cv::Rect spotuprect=cv::Rect(0,0,spot.cols,(int)(spot.rows*spotpos));
		cv::Rect spotdownrect=cv::Rect(0,(int)(spot.rows*spotpos),spot.cols,spot.rows-(int)(spot.rows*spotpos));
		zz::zScanPixel(soil,soilleftrect,&soilleft);	
		zz::zScanPixel(soil,soilrightrect,&soilright);	
		zz::zScanPixel(spot,spotuprect,&spotup);	
		zz::zScanPixel(spot,spotdownrect,&spotdown);	
		
		//draw line
		cv::rectangle(soil,soilleftrect,cv::Scalar(128));
		cv::rectangle(soil,soilrightrect,cv::Scalar(128));
		cv::rectangle(frame,soilleftrect,cv::Scalar(255,0,0));
		cv::rectangle(frame,soilrightrect,cv::Scalar(255,0,0));
		cv::rectangle(spot,spotuprect,cv::Scalar(128));
		cv::rectangle(spot,spotdownrect,cv::Scalar(128));
		cv::rectangle(frame,spotuprect,cv::Scalar(0,0,255));
		cv::rectangle(frame,spotdownrect,cv::Scalar(0,0,255));
		//end draw line
		
		cv::imshow("Raw",frame);
		cv::imshow("Soil",soil);
		cv::imshow("Spot",spot);
		
		std::ostringstream sbuff;
		sbuff<< soilleft << '\t' << soilright << "\r\n";
		zz::zSerial::SerialWrite(sbuff.str().data());

		tt=(double)cv::getTickCount()-tt;
		tt=tt/cv::getTickFrequency()*1000;

		std::cout<< "l: " << soilleft << "\tr: " << soilright;
		std::cout<< "\tu: " << spotup << "\td: " << spotdown;
		std::cout<< "\tt:" << tt << "ms\n";

		keycode=cv::waitKey(1);
	}

	zz::zSerial::SerialClose();

	return 0;
}


