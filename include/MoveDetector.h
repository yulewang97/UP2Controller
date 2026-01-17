#include <iostream>
#include "ThreadController.h"

// 多线程全局变量
extern VideoCapture globalCapture;
extern vector<Mat> globalFrames;
extern int lastestFrame;
extern int oldFrame;

Mat MoveDetect(Mat oldFrame, Mat lastestFrame);
int GreenDetect(Mat src);
Mat stdMoveDetect(Mat oldFrame, Mat lastestFrame);