// IntelProgram.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#using "Car.dll"
#include <iostream>
#include <conio.h>	//_getch()函数所在库，后面的项目中应该用不上
#include <time.h>
#include <windows.h>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <math.h>
#include <fstream> 

#pragma warning(disable:4996)

using namespace Car;
using namespace std;
using namespace cv;

//从球体内部来看
//以0度经线为基准，左为东经，右为西经
//设东经为负，西经为正（都可，互换）


//经度的误差
#define Longitude_Error 20

//前进和旋转的速度
#define fs 1.0	//forward speed
#define ts 1.0	//turn speed

//小车做出后退或前进的时间（毫秒为单位)
#define actime 1000

//小车状态：
#define STAY 0		//保持不动
#define FORWARD 1	//向前移动
#define BACK 2		//向后移动
#define ROTATE 3	//原地旋转

//图像处理方面的宏定义
#define PI 3.141592
#define DEPTH 422500     /*数据深度，即存储单元的个数*/
#define WIDTH 10       /*存储单元的宽度*/
#define RADIUS 640
#define DIAMETER 1280


double opencvRed(IplImage*);


double opencvRed(IplImage* hsv) {
	bool over = 0;

	CvScalar s_hsv;
	for (int i = 0; i < 1280; i += 32)
		for (int j = 0; j < 2560; j += 32)
		{
			for (int k = 0; k <32 && over == 0; k++)
				for (int m = 0; m < 32 && over == 0; m++)
				{
					s_hsv = cvGet2D(hsv, i + k, j + m);
					if ((!(((s_hsv.val[0]>0) && (s_hsv.val[0]<8)) || (s_hsv.val[0]>120) && (s_hsv.val[0]<180))) || s_hsv.val[1] < 80)
					{
						//s.val[0] = 0;
						//s.val[1] = 0;
						//s.val[2] = 0;
						//cvSet2D(hsv, i, j, s);
						over = 1;
					}
				}

			if (over == 0) {
				//cout << j  << endl;
				return j;
			}
			else
				over = 0;

		}
	return -1;
}



double opencvFire(IplImage*);

double opencvFire(IplImage* stretch) {
	bool over = 0;

	uchar* data = (uchar*)stretch->imageData;
	int step = stretch->widthStep;

	int number = 0, r, g, b = 0;
	double S, maxv, minv;

	for (int i = 0; i < DIAMETER; i += 32)
		for (int j = 0; j < DIAMETER * 2; j += 32)
		{
			for (int k = 0; k <32 && over == 0; k++)
				for (int m = 0; m < 32 && over == 0; m++)
				{

					r = data[(i + k)*step + (j + m) * 3 + 2];
					g = data[(i + k)*step + (j + m) * 3 + 1];
					b = data[(i + k)*step + (j + m) * 3];
					maxv = max(max(b, g), r);
					minv = min(min(b, g), r);
					S = (maxv - minv) / maxv;
					if ((r > g) && (g > b) && (r > 100) && (S >((255 - r) * 2.5 / 100)))
					{
						number++;
						if (number > 550)
							over = 1;
					}
				}
			number = 0;

			if (over == 1) {
				//cout << j  << endl;
				return j;
			}


		}
	return -1;
}

//项目主函数
int main()
{
	//init，本项目中使用mode3来完成小车移动
	PcanControl ^car = gcnew PcanControl();
	car->SetMode(car->Mode3);


	//有没有检测到
	bool detected = false;
	//检测到的是红(1)还是绿(0)
	bool danger = true;

	//小车目前的状态
	int state = 0;

	//状态测试
	vector<string> debugtest;
	debugtest.push_back("不动");
	debugtest.push_back("向前");
	debugtest.push_back("向后");
	debugtest.push_back("旋转");


	//the detected thing's longitude
	double longitude = 0;

	//图像处理部分的init
	double rotation = 0;
	IplImage* output = cvCreateImage(cvSize(2560, 1280), 8, 3);

	IplImage* hsv = cvCreateImage(cvSize(2560, 1280), 8, 3);

	uchar* data;

	uchar* dataOut = (uchar *)output->imageData;

	unsigned int* stretchData = new unsigned int[DIAMETER *DIAMETER];

	double rate;

	VideoCapture capture;
	Mat frame, showImg;
	IplImage * src;

	int redPart = 0;

	for (int row = 0; row < RADIUS; row++) {
		rate = sqrt(2 * row * RADIUS - row*row) / RADIUS;
		for (int col = 0; col < DIAMETER; col++) {
			if (col <RADIUS)
				stretchData[row*DIAMETER + col] = -abs(col - RADIUS)*rate + 720;
			else
				stretchData[row*DIAMETER + col] = abs(col - RADIUS)*rate + 720;
		}
	}

	for (int row = RADIUS; row < DIAMETER; row++) {
		rate = sqrt(2 * (2 * RADIUS - row) * RADIUS - (2 * RADIUS - row)*(2 * RADIUS - row)) / RADIUS;
		for (int col = 0; col < DIAMETER; col++) {
			if (col <RADIUS)
				stretchData[row*DIAMETER + col] = -abs(col - RADIUS)*rate + 720;
			else
				stretchData[row*DIAMETER + col] = abs(col - RADIUS)*rate + 720;
		}
	}

	int stepOut = output->widthStep / sizeof(uchar);
	int stepIn = 2880 * 3;

	//打开串流获取图像
	capture.open("rtmp://192.168.0.100/live/insta360");

	if (!capture.isOpened())
	{
		cout << "摄像头打开失败！" << endl;
		return 0;
	}

	char opt;
	while (1) {
		//设置程序结束的按钮
		if (_kbhit()) {
			opt = _getch();
			if (opt == VK_ESCAPE) {
				//检测到esc键按下，停止小车
				car->SetSpeed(0, 0);
				car->Stop();
				capture.release();
				return 0;
			}
		}

		//更新经度值和检测到的颜色
		//获取一帧
		capture >> frame;
		src = &IplImage(frame);

		data = (uchar *)src->imageData;

		for (int row = 0; row < (output->height); row++) {
			for (int col = 0; col < (output->width) / 2; col++) {
				dataOut[row*stepOut + col * 3] = data[(row + 80)*stepIn + (stretchData[row*DIAMETER + col]) * 3];
				dataOut[row*stepOut + col * 3 + 1] = data[(row + 80)*stepIn + (stretchData[row*DIAMETER + col]) * 3 + 1];
				dataOut[row*stepOut + col * 3 + 2] = data[(row + 80)*stepIn + (stretchData[row*DIAMETER + col]) * 3 + 2];
			}
		}
		for (int row = 0; row < (output->height); row++) {
			for (int col = (output->width) / 2; col < (output->width); col++) {
				dataOut[row*stepOut + col * 3] = data[(row + 80)*stepIn + (stretchData[row*DIAMETER + col - DIAMETER] + 1440) * 3];
				dataOut[row*stepOut + col * 3 + 1] = data[(row + 80)*stepIn + (stretchData[row*DIAMETER + col - DIAMETER] + 1440) * 3 + 1];
				dataOut[row*stepOut + col * 3 + 2] = data[(row + 80)*stepIn + (stretchData[row*DIAMETER + col - DIAMETER] + 1440) * 3 + 2];
			}
		}

		cvCvtColor(output, hsv, CV_BGR2HSV);

		//redPart = opencvFire(output);
		redPart = opencvRed(hsv);

		if (redPart != -1) {
			if (redPart < 1920)
				rotation = (redPart - 640) * 360 / 2560;
			else
				rotation = (redPart - 640 - 2560) * 360 / 2560;
			cout << "longitude: " << rotation << endl;
		}

		//rotation就是控制小车需要的那个角度

		longitude = rotation;
		danger = true;


		//判断是否有改变
		if (redPart != -1) {
			//detected
			detected = true;
		}

		//如果有经度改变，小车需要做出相应的运动
		if (detected) {

			//如果小车目前的状态是停止,那么小车需要进行旋转，然后移动
			if (state == STAY) {
				//修改状态为旋转
				state = ROTATE;
				//更新进入的时间
			}

			//如果小车目前的状态为前进，那么阻断2s，让小车移动
			else if (state == FORWARD) {

				car->SetSpeed(fs, fs);
				car->Start();

				Sleep(actime);
				//阻断两秒之后，默认小车已经到达安全位置，如果没有，下一层循环会让小车继续运动
				car->SetSpeed(0, 0);
				car->Stop();
				detected = false;
				longitude = 0;
				state = STAY;
			}
			else if (state == BACK) {

				car->SetSpeed(-fs, -fs);
				car->Start();

				Sleep(actime);
				//阻断两秒之后，默认小车已经脱离危险位置，如果没有，下一层循环会让小车继续运动
				car->SetSpeed(0, 0);
				car->Stop();
				detected = false;
				longitude = 0;
				state = STAY;
			}
			else {
				//state == ROTATE
				//小车开始旋转
				//判断小车应该有什么运动状态
				if (abs(longitude) < Longitude_Error) {
					//保持当前状态后退或者前进
					if (danger) {
						state = BACK;
					}
					else {
						state = FORWARD;
					}
					longitude = 0;
					car->SetSpeed(0, 0);
					car->Stop();
				}
				//可能的问题，在180°到-180°之间的时候，会出现正负的跳变，形成忽左忽右的旋转困境
				//需要注意条件的写法。
				else if (abs(longitude) > 140) {
					car->SetSpeed(ts, -ts);
					car->Start();
				}
				else if (longitude > 0) {
					//西经，小车右转
					car->SetSpeed(ts, -ts);
					car->Start();

				}
				else {
					//东经，小车左转
					car->SetSpeed(-ts, ts);
					car->Start();

				}
			}
		}
		else {
			//没有检测到，或者已经脱离检测范围，小车保持不动
			car->SetSpeed(0, 0);
			car->Stop();
			longitude = 0;
			state = STAY;
		}

		//输出小车目前的状态
		cout << "state:  " << debugtest[state] << endl << endl;

		waitKey(30);
	}
}
