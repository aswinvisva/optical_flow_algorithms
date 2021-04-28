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

void preprocessFrame(Mat &frame) 
{
    cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
    cv::resize(frame, frame, cv::Size(512, 512));    
}


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

    int t = 0;
    Mat prev;

    while(1) {
        cap >> frame;

        Mat Ix, Iy, It, flow;
        
        preprocessFrame(frame);

        if(t > 0) {
            getImageDerivatives(frame, prev, Ix, Iy, It);

            cv::Mat u = cv::Mat::zeros(Ix.size(), CV_32F);
            cv::Mat v = cv::Mat::zeros(Ix.size(), CV_32F);

            leastSquaresEstimation(Ix, Iy, It, u, v);

            cv::Mat magnitude, angle, magn_norm;

            cartToPolar(u, v, magnitude, angle, true);
            normalize(magnitude, magn_norm, 0.0f, 1.0f, NORM_MINMAX);

            angle *= ((1.f / 360.f) * (180.f / 255.f));

            Mat norm_u, norm_v;

            normalize(u, norm_u, 0.0f, 1.0f, NORM_MINMAX);
            normalize(v, norm_v, 0.0f, 1.0f, NORM_MINMAX);

             Mat _hsv[3], hsv, hsv8, bgr;
            _hsv[0] = angle;
            _hsv[1] = Mat::ones(angle.size(), CV_32F);
            _hsv[2] = magn_norm;
            merge(_hsv, 3, hsv);
            hsv.convertTo(hsv8, CV_8U, 255.0);
            cvtColor(hsv8, bgr, COLOR_HSV2BGR);

            imshow("Optical Flow", bgr);
        }

        imshow("Preview", frame);

        waitKey(0);

        prev = frame.clone();

        t++;
    }
}