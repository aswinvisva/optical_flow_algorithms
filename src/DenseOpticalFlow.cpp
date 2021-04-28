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

using namespace cv;
using namespace std;

cv::Mat convolve3x3(cv::Mat frame, cv::Mat kernel) {
    /*
    Perform a 3x3 Convolution of an image with a given kernel
    */

    frame.convertTo(frame, CV_32F);

    Mat res = cv::Mat::zeros(frame.size(), CV_32F);

    cout << res.size << endl;

    for(int i = 1; i < frame.rows-2; i++) {
        for(int j = 1; j < frame.cols-2; j++) {
            cv::Rect roi(j, i, 3, 3);
            cv::Mat imgRoi = frame(roi);
            cv::Mat product = imgRoi.mul(kernel);

            res.at<float>(i+1, j+1) = cv::sum(product)[0];
        }
    }

    return res;
}

void getSobelDerivative(cv::Mat frame, cv::Mat &Ix, cv::Mat &Iy) {
    /*
    Get the Sobel estimations of the Ix, Iy matrices
    */

    cv::Mat sobelX, sobelY, Gx, Gy;

    getDerivKernels(sobelX, sobelY, 0, 1, 3);
    
    cv:Mat sobelYFlipped;

    cv::flip(sobelY, sobelYFlipped, 0);

    Gx = sobelX*sobelYFlipped.t();
    Gy = sobelYFlipped*sobelX.t();

    cout << "Gx = " << endl << " " << Gx << endl << endl;
    cout << "Gy = " << endl << " " << Gy << endl << endl;

    Ix = convolve3x3(frame, Gx);
    Iy = convolve3x3(frame, Gy);

    cv::imshow("Ix", Ix);
    cv::imshow("Iy", Iy);
    cv::waitKey(0);
}
