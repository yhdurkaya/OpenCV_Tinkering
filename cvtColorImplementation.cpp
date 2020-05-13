//
// Created by yhd on 29.04.2020.
//

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

cv::Mat convertBGRtoGray(cv::Mat img){
    cv::Mat grayImage = cv::Mat::zeros(cv::Size(img.cols, img.rows), CV_8U);

    for(int i = 0; i < img.rows; ++i){
        for(int j = 0; j < img.cols; ++j){
            grayImage.at<char>(i, j) = 0.114 * (img.at<cv::Vec3b>(i, j)[0]) + 0.587 * (img.at<cv::Vec3b>(i, j)[1]) + 0.299 * (img.at<cv::Vec3b>(i, j)[2]);
        }
    }
    return grayImage;
}

cv::Mat convertBGRtoHSV(cv::Mat image){
    cv::Mat hsvImage = cv::Mat::zeros(cv::Size(image.cols, image.rows), CV_8UC3);
    double h, s, v, b, g, r, min;

    for(int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            b = (static_cast<double>(image.at<cv::Vec3b>(i, j)[0])) / 255;
            g = (static_cast<double>(image.at<cv::Vec3b>(i, j)[1])) / 255;
            r = (static_cast<double>(image.at<cv::Vec3b>(i, j)[2])) / 255;

            v = ((b > g) ? b : g) > r ? ((b > g) ? b : g) : r;

            if (v == 0) {
                s = 0;
            } else {
                min = ((b < g) ? b : g) < r ? ((b < g) ? b : g) : r;
                s = (v - min) / v;
            }

            if (v == r) {
                h = (60 * (g - b)) / (v - min);
            } else if (v == g) {
                h = 120 + (60 * (b - r)) / (v - min);
            } else if (v == b) {
                h = 240 + (60 * (r - g)) / (v - min);
            }

            if (h < 0) {
                h += 360;
            }

            hsvImage.at<cv::Vec3b>(i, j)[0]= v * 255;
            hsvImage.at<cv::Vec3b>(i, j)[1] = s * 255;
            hsvImage.at<cv::Vec3b>(i, j)[2] = h / 2;
        }
    }

    return hsvImage;
}

int main(){
    std::string DATA_PATH = "../data/images/";
    cv::Mat image = cv::imread(DATA_PATH + "trump.jpg");


    //cv::Mat grayscaleImage = convertBGRtoGray(image);

    //cv::imshow("Original Image", image);
    //cv::imshow("Grayscaled Image", grayscaleImage);

    //std::cout << "Width and height of the original image : " << image.cols << "x" << image.rows << std::endl;
    //std::cout << "Width and height of the grayscale image : " << grayscaleImage.cols << "x" << grayscaleImage.rows << std::endl;

    cv::Mat hsvImg1 = cv::Mat::zeros(cv::Size(image.cols, image.rows), CV_8UC3);

    cv::cvtColor(image, hsvImg1, cv::COLOR_BGR2HSV);

    cv::Mat hsvImg2 = convertBGRtoHSV(image);

    cv::Mat diffHSV = cv::Mat::zeros(cv::Size(image.cols, image.rows), CV_8UC3);

    cv::absdiff(hsvImg1, hsvImg2, diffHSV);

    cv::imshow("OpenCV HSV", hsvImg1);
    cv::imshow("MyHSV", hsvImg2);
    cv::imshow("dfif", diffHSV);

    cv::waitKey(0);
}