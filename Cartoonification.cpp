//
// Created by yhd on 19.03.2020.
//

#include <opencv2/opencv.hpp>

std::string DATA_PATH = "../data/images/";

int main(){
    cv::Mat sourceImage, grayImage, edges, bilateralFiltered, cartoonifiedImage;

    sourceImage = cv::imread(DATA_PATH + "trump.jpg");

    cv::cvtColor(sourceImage, grayImage, cv::COLOR_BGR2GRAY);
    cv::medianBlur(grayImage, grayImage, 5);
    cv::adaptiveThreshold(grayImage, edges, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 7 ,5);

    cv::bilateralFilter(sourceImage, bilateralFiltered, 9, 300, 300);

    cv::bitwise_and(bilateralFiltered, bilateralFiltered, cartoonifiedImage, edges);

    cv::imshow("Source Image", sourceImage);
    cv::imshow("Gray Image", grayImage);
    cv::imshow("Edges", edges);
    cv::imshow("Cartoonified Image", cartoonifiedImage);
    cv::waitKey(0);
}