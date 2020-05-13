//
// Created by yhd on 19.03.2020.
//

#include <iostream>

#include <opencv2/opencv.hpp>

std::string DATA_PATH = "../data/images/";

cv::Mat img, mask;

void mouseClick(int event, int x, int y, int flags, void* userdata){

    if  ( event == cv::EVENT_LBUTTONDOWN )
    {
        circle(mask, cv::Point(x, y), 1, cv::Scalar(255, 255, 255), 15, cv::LINE_AA);
        circle(img, cv::Point(x, y), 1, cv::Scalar(255, 255, 255), 15, cv::LINE_AA);

        cv::inpaint(img, mask, img, 3, cv::INPAINT_NS);
        cv::imshow("Blemish Removal", img);
    }
}

int main(){
//seamless cloning
    img = imread("/blemish.png", cv::IMREAD_COLOR);
    mask = img.clone();

    mask = cv::Mat::zeros(mask.size(), CV_8U);

    cv::namedWindow("Blemish Removal", 1);
    cv::imshow("Blemish Removal", img);

    cv::setMouseCallback("Blemish Removal", mouseClick, NULL);

    cv::waitKey(0);



}
