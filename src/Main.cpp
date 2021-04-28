#include <stdio.h>
// #include <io>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <stack>
#include <sys/stat.h>
#include <string>

#include "DenseOpticalFlow.hpp"

using namespace cv;
using namespace std;


int main(int argc, char** argv)
{
    char* videoPath = argv[1];

    Mat frame;

    VideoCapture cap;
    cap.open(videoPath);

    if(!cap.isOpened()) {
        cout << "Error opening video stream or file" << endl;
        return -1;
    }

    while(1) {
        cap >> frame;

        Mat gray, Ix, Iy;

        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        getSobelDerivative(gray, Ix, Iy);

        imshow("Preview", frame);

        waitKey(0);
    }
}