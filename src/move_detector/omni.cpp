#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <math.h>
#include <iostream>

using namespace std;
using namespace cv;
void pictureConvert(IplImage* src);


int main() {
	VideoCapture capture;
	Mat frame, showImg;
	IplImage * src;
	int number = 0;
	//pictureConvert(src);
	//capture.open("rtmp://192.168.77.100/live/insta360");

	//if (!capture.isOpened())
	//{
	//	cout << "ÉãÏñÍ·´ò¿ªÊ§°Ü£¡" << endl;
	//	return 0;
	//}
	src = cvLoadImage("C:/Users/12207/Documents/Visual Studio 2015/Projects/omnifpga/omnifpga/fart.jpg", 0);
	//namedWindow("Camera");
	//namedWindow("Camera", CV_WINDOW_NORMAL);
	char* data = (char *)src->imageData;


	capture >> frame;
	//imshow("Camera", frame);

	//imwrite("fart1.jpg", img);
	/*while (1) {
		number++;
		capture >> frame;

	//imshow("Camera", frame);

	//imwrite("fart.jpg",frame);

		src = &IplImage(frame);

		//cout << src->imageSize << endl;
	//pictureConvert(src);
	//qDebug("i have a picture");
	/*try {
	imshow("Camera", frame);
	}
	catch (Exception e) {
	//qDebug("yes");
	//qDebug(e.err.c_str());
	}*/
	//cvResizeWindow("Camera", 200, 200);

	//waitKey(1);
	//}
	String outname = "fart1.jpg";
	cvSaveImage(outname.c_str(), src);
	return 0;
}


