#include <stdio.h>
// #include <io>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <stack>
#include <sys/stat.h>
#include <string>

using namespace cv;
using namespace std;

cv::Mat convolve2D(cv::Mat frame, cv::Mat kernel) {
    /*
    Perform a 2D Convolution of an image with a given kernel
    */

    frame.convertTo(frame, CV_32F);

    Mat res = cv::Mat::zeros(frame.size(), CV_32F);

    for(int i = 1; i < frame.rows-(kernel.rows-1); i++) {
        for(int j = 1; j < frame.cols-(kernel.cols-1); j++) {
            cv::Rect roi(j, i, kernel.rows, kernel.cols);
            cv::Mat imgRoi = frame(roi);
            cv::Mat product = imgRoi.mul(kernel);

            res.at<float>(i+1, j+1) = cv::sum(product)[0];
        }
    }

    return res;
}

void getImageDerivatives(cv::Mat frame, cv::Mat prev, cv::Mat &Ix, cv::Mat &Iy, cv::Mat &It) {
    /*
    Get the Ix, Iy, It gradients
    */

    float xData[4] = {-1, 1, -1, 1};
    cv::Mat Gx = cv::Mat(2, 2, CV_32F, xData);

    float yData[4] = {-1, -1, 1, 1};
    cv::Mat Gy = cv::Mat(2, 2, CV_32F, yData);

    float t1Data[4] = {-1, -1, -1, -1};
    cv::Mat Gt1 = cv::Mat(2, 2, CV_32F, t1Data);

    float t2Data[4] = {1, 1, 1, 1};
    cv::Mat Gt2 = cv::Mat(2, 2, CV_32F, t2Data);

    Ix = convolve2D(frame, Gx) + convolve2D(prev, Gx);
    Iy = convolve2D(frame, Gy) + convolve2D(prev, Gy);
    It = convolve2D(prev, Gt1) + convolve2D(frame, Gt2);
}

void leastSquaresEstimation(cv::Mat Ix, cv::Mat Iy, cv::Mat It, cv::Mat &u, cv::Mat &v) {
    /*
    Compute the least squares estimation of the u(x) matrix
    */

    for(int i = 2; i < u.rows - 5; i++) {
        for(int j = 2; j < u.cols - 5; j++) {

            cv::Rect roi(j-2, i-2, 5, 5);

            cv::Mat A_x = Ix(roi).clone().reshape(1, 25);
            cv::Mat A_y = Iy(roi).clone().reshape(1, 25);
            cv::Mat b = It(roi).clone().reshape(1, 25);

            cv::Mat A;

            cv::hconcat(A_x, A_y, A);

            cv::Mat res = (A.t() * A).inv() * A.t() * b;

            u.at<float>(i, j) = res.at<float>(0, 0);
            v.at<float>(i, j) = res.at<float>(1, 0);
        }
    }
}
