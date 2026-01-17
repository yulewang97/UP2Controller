#include "ThreadController.h"

VideoCapture globalCapture;
vector<Mat> globalFrames(3);
int lastestFrame = 0;
int oldFrame = 1;
int swapFrame = 2;

DWORD WINAPI readFrame(LPVOID lpParam) {
	int swapTemp;
	while (true) {
		globalCapture.grab();
		globalCapture.retrieve(globalFrames[swapFrame]);
		// No lock swap
		swapTemp = oldFrame;
		oldFrame = lastestFrame;
		lastestFrame = swapFrame;
		swapFrame = swapTemp;
	}
	return 0;
}