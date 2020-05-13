//
// Created by yhd on 18.03.2020.
//

//Pencil-Sketch filter - some sort of gradient or laplacion, not canny edge detector.
//Cartoonification filter - some sort of edge preserving filter and some sort of edge detection

#include <iostream>

#include <opencv2/opencv.hpp>

std::string DATA_PATH = "../data/images/";

int main(){
    cv::Mat sourceImage, invertedImage, blurredImage, pencilSketch;
    sourceImage = cv::imread(DATA_PATH + "trump.jpg", cv::IMREAD_GRAYSCALE);


    cv::bitwise_not(sourceImage, invertedImage);

    GaussianBlur(invertedImage, blurredImage, cv::Size(17, 17), 0, 0);

    //cv::divide(sourceImage, blurredImage, pencilSketch, 256.0);
    cv::addWeighted(sourceImage, 1, blurredImage, 1, 0 ,pencilSketch);

    cv::imshow("Source Image", sourceImage);
    cv::imshow("Inverted Image", invertedImage);
    cv::imshow("Gaussian Blurred Image", blurredImage);
    cv::imshow("Pencil Sketch", pencilSketch);
    cv::waitKey(0);
}