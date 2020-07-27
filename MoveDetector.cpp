#include "MoveDetector.h"

Mat MoveDetect(Mat _oldFrame, Mat _newFrame)
{
	//Mat result = lastestFrame.clone();
	//1.将background和frame转为灰度图
	Mat gray1, gray2;
	cvtColor(_oldFrame, gray1, CV_BGR2GRAY);
	cvtColor(_newFrame, gray2, CV_BGR2GRAY);
	//2.将background和frame做差
	Mat diff;
	absdiff(gray1, gray2, diff);
	//imshow("diff", diff);
	//3.对差值图diff_thresh进行阈值化处理
	Mat diff_thresh;
	threshold(diff, diff_thresh, 50, 255, CV_THRESH_BINARY);
	//imshow("diff_thresh", diff_thresh);
	Mat result;
	cvtColor(diff_thresh, diff_thresh, CV_GRAY2BGR);
	bitwise_and(diff_thresh, _newFrame, result);
	return result;
}

bool PointCheck(Vec3b v3) {
	if (v3[1] > v3[0] + v3[2] && v3[1] >= 100) {
		return true;
	}
	else {
		return false;
	}
}

int GreenDetect(Mat src) {
	int row = src.rows;
	int col = src.cols;

	bool ret;

	//遍历整张图像，读取颜色为绿色的部分（G > R + B）9x9
	for (int i = 4; i < row - 5; ++i) {
		for (int j = 4; j < col - 5; ++j) {
			ret = PointCheck(src.at<Vec3b>(i - 4, j - 4))
				&& PointCheck(src.at<Vec3b>(i - 4, j - 3))
				&& PointCheck(src.at<Vec3b>(i - 4, j - 2))
				&& PointCheck(src.at<Vec3b>(i - 4, j - 1))
				&& PointCheck(src.at<Vec3b>(i - 4, j))
				&& PointCheck(src.at<Vec3b>(i - 4, j + 1))
				&& PointCheck(src.at<Vec3b>(i - 4, j + 2))
				&& PointCheck(src.at<Vec3b>(i - 4, j + 3))
				&& PointCheck(src.at<Vec3b>(i - 4, j + 4))

				&& PointCheck(src.at<Vec3b>(i - 3, j - 4))
				&& PointCheck(src.at<Vec3b>(i - 3, j - 3))
				&& PointCheck(src.at<Vec3b>(i - 3, j - 2))
				&& PointCheck(src.at<Vec3b>(i - 3, j - 1))
				&& PointCheck(src.at<Vec3b>(i - 3, j))
				&& PointCheck(src.at<Vec3b>(i - 3, j + 1))
				&& PointCheck(src.at<Vec3b>(i - 3, j + 2))
				&& PointCheck(src.at<Vec3b>(i - 3, j + 3))
				&& PointCheck(src.at<Vec3b>(i - 3, j + 4))

				&& PointCheck(src.at<Vec3b>(i - 2, j - 4))
				&& PointCheck(src.at<Vec3b>(i - 2, j - 3))
				&& PointCheck(src.at<Vec3b>(i - 2, j - 2))
				&& PointCheck(src.at<Vec3b>(i - 2, j - 1))
				&& PointCheck(src.at<Vec3b>(i - 2, j))
				&& PointCheck(src.at<Vec3b>(i - 2, j + 1))
				&& PointCheck(src.at<Vec3b>(i - 2, j + 2))
				&& PointCheck(src.at<Vec3b>(i - 2, j + 3))
				&& PointCheck(src.at<Vec3b>(i - 2, j + 4))

				&& PointCheck(src.at<Vec3b>(i - 1, j - 4))
				&& PointCheck(src.at<Vec3b>(i - 1, j - 3))
				&& PointCheck(src.at<Vec3b>(i - 1, j - 2))
				&& PointCheck(src.at<Vec3b>(i - 1, j - 1))
				&& PointCheck(src.at<Vec3b>(i - 1, j))
				&& PointCheck(src.at<Vec3b>(i - 1, j + 1))
				&& PointCheck(src.at<Vec3b>(i - 1, j + 2))
				&& PointCheck(src.at<Vec3b>(i - 1, j + 3))
				&& PointCheck(src.at<Vec3b>(i - 1, j + 4))

				&& PointCheck(src.at<Vec3b>(i, j - 4))
				&& PointCheck(src.at<Vec3b>(i, j - 3))
				&& PointCheck(src.at<Vec3b>(i, j - 2))
				&& PointCheck(src.at<Vec3b>(i, j - 1))
				&& PointCheck(src.at<Vec3b>(i, j))
				&& PointCheck(src.at<Vec3b>(i, j + 1))
				&& PointCheck(src.at<Vec3b>(i, j + 2))
				&& PointCheck(src.at<Vec3b>(i, j + 3))
				&& PointCheck(src.at<Vec3b>(i, j + 4))

				&& PointCheck(src.at<Vec3b>(i + 1, j - 4))
				&& PointCheck(src.at<Vec3b>(i + 1, j - 3))
				&& PointCheck(src.at<Vec3b>(i + 1, j - 2))
				&& PointCheck(src.at<Vec3b>(i + 1, j - 1))
				&& PointCheck(src.at<Vec3b>(i + 1, j))
				&& PointCheck(src.at<Vec3b>(i + 1, j + 1))
				&& PointCheck(src.at<Vec3b>(i + 1, j + 2))
				&& PointCheck(src.at<Vec3b>(i + 1, j + 3))
				&& PointCheck(src.at<Vec3b>(i + 1, j + 4))

				&& PointCheck(src.at<Vec3b>(i + 2, j - 4))
				&& PointCheck(src.at<Vec3b>(i + 2, j - 3))
				&& PointCheck(src.at<Vec3b>(i + 2, j - 2))
				&& PointCheck(src.at<Vec3b>(i + 2, j - 1))
				&& PointCheck(src.at<Vec3b>(i + 2, j))
				&& PointCheck(src.at<Vec3b>(i + 2, j + 1))
				&& PointCheck(src.at<Vec3b>(i + 2, j + 2))
				&& PointCheck(src.at<Vec3b>(i + 2, j + 3))
				&& PointCheck(src.at<Vec3b>(i + 2, j + 4))

				&& PointCheck(src.at<Vec3b>(i + 3, j - 4))
				&& PointCheck(src.at<Vec3b>(i + 3, j - 3))
				&& PointCheck(src.at<Vec3b>(i + 3, j - 2))
				&& PointCheck(src.at<Vec3b>(i + 3, j - 1))
				&& PointCheck(src.at<Vec3b>(i + 3, j))
				&& PointCheck(src.at<Vec3b>(i + 3, j + 1))
				&& PointCheck(src.at<Vec3b>(i + 3, j + 2))
				&& PointCheck(src.at<Vec3b>(i + 3, j + 3))
				&& PointCheck(src.at<Vec3b>(i + 3, j + 4))

				&& PointCheck(src.at<Vec3b>(i + 4, j - 4))
				&& PointCheck(src.at<Vec3b>(i + 4, j - 3))
				&& PointCheck(src.at<Vec3b>(i + 4, j - 2))
				&& PointCheck(src.at<Vec3b>(i + 4, j - 1))
				&& PointCheck(src.at<Vec3b>(i + 4, j))
				&& PointCheck(src.at<Vec3b>(i + 4, j + 1))
				&& PointCheck(src.at<Vec3b>(i + 4, j + 2))
				&& PointCheck(src.at<Vec3b>(i + 4, j + 3))
				&& PointCheck(src.at<Vec3b>(i + 4, j + 4));

			if (ret) {
				return j;
			}
		}
	}
	return -1;
}

Mat stdMoveDetect(Mat oldFrame, Mat lastestFrame)
{
	Mat result = lastestFrame.clone();
	//1.将background和frame转为灰度图
	Mat gray1, gray2;
	cvtColor(oldFrame, gray1, CV_BGR2GRAY);
	cvtColor(lastestFrame, gray2, CV_BGR2GRAY);
	//2.将background和frame做差
	Mat diff;
	absdiff(gray1, gray2, diff);
	//imshow("diff", diff);
	//3.对差值图diff_thresh进行阈值化处理
	Mat diff_thresh;
	threshold(diff, diff_thresh, 50, 255, CV_THRESH_BINARY);
	//imshow("diff_thresh", diff_thresh);
	//4.腐蚀
	Mat kernel_erode = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat kernel_dilate = getStructuringElement(MORPH_RECT, Size(18, 18));
	erode(diff_thresh, diff_thresh, kernel_erode);
	//imshow("erode", diff_thresh);
	//5.膨胀
	dilate(diff_thresh, diff_thresh, kernel_dilate);
	//imshow("dilate", diff_thresh);
	//6.查找轮廓并绘制轮廓
	vector<vector<Point>> contours;
	findContours(diff_thresh, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	drawContours(result, contours, -1, Scalar(0, 0, 255), 2);//在result上绘制轮廓
															 //7.查找正外接矩形
	vector<Rect> boundRect(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		boundRect[i] = boundingRect(contours[i]);
		rectangle(result, boundRect[i], Scalar(0, 255, 0), 2);//在result上绘制正外接矩形
	}
	return result;//返回result
}
