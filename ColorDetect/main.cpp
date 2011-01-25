#include "stdafx.h"

#include "zz.hpp"

#include <iostream>

cv::Point select_origin;
cv::Rect select_rect;
bool select_on=false;

int _tmain(int argc, _TCHAR* argv[])
{
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

	cv::VideoCapture cap;

	std::cout << "initial webcam...";
	cap.open(0);
	if(!cap.isOpened())
	{
		std::printf("err init webcam!");
		std::getchar();
		return 0;
	}
	std::cout << "OK\n";

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
	
	cv::Mat frame,soil,spot;

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

		zz::zColor(frame,soil,soilhmin,soilhmax,soilsmin,soilsmax,soilvmin,soilvmax,soilerode,soildilate);
		zz::zColor(frame,spot,spothmin,spothmax,spotsmin,spotsmax,spotvmin,spotvmax,spoterode,spotdilate);

		cv::imshow("Raw",frame);
		cv::imshow("Soil",soil);
		cv::imshow("Spot",spot);

		int left=0;
		for(int row=0;row<soil.rows;row++)
		{
			uchar *inp=soil.ptr<uchar>(row);
			for(int col=0;col<soil.cols/2;col++)
			{
				if(*inp==0)
				{
				}
				else
				{
					left++;
				}
				inp++;
			}
		}

		tt=(double)cv::getTickCount()-tt;
		tt=tt/cv::getTickFrequency()*1000;
		std::cout<< "t:" << tt << "ms\n";
		std::cout<< ' ' << left;
		keycode=cv::waitKey(1); }

	std::cout << keycode;
	cv::destroyWindow("Raw");
	cv::destroyWindow("Soil");
	cv::destroyWindow("SoilSetting1");
	cv::destroyWindow("SoilSetting2");
	cv::destroyWindow("Spot");
	cv::destroyWindow("SpotSetting1");
	cv::destroyWindow("SpotSetting2");

	return 0;
}

