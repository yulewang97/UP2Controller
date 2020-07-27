#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <windows.h>
#include <vector>

using namespace std;
using namespace cv;

DWORD WINAPI readFrame(LPVOID lpParam);