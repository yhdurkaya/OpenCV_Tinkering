//
// Created by yhd on 21.03.2020.
//

#include <iostream>

#include <opencv2/opencv.hpp>

std::string DATA_PATH = "../data/images/";

cv::Mat img, imgGray, grad;

void mouseClick(int event, int x, int y, int flags, void* userdata){

    if  ( event == cv::EVENT_LBUTTONDOWN )
    {

    }
}

int main(){
//seamless cloning
    img = imread(DATA_PATH + "blemish.png", cv::IMREAD_COLOR);

    cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);

    cv::GaussianBlur(img, img, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);

    cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);

    cv::Mat gradX, gradY, absGradX, absGradY;

    cv::Sobel(imgGray, gradX, CV_16S, 1, 0, 3, 1, 0 , cv::BORDER_DEFAULT);
    cv::Sobel(imgGray, gradY, CV_16S, 0, 1, 3, 1, 0 , cv::BORDER_DEFAULT);

    convertScaleAbs(gradX, absGradX);
    convertScaleAbs(gradY, absGradY);

    cv::addWeighted(absGradX, 0.5, absGradY, 0.5, 0, grad);

    cv::namedWindow("Blemish Removal", 1);
    cv::imshow("Blemish Removal", img);

    cv::imshow("GRAD", grad);

    cv::setMouseCallback("Blemish Removal", mouseClick, NULL);

    cv::waitKey(0);



}