//
// Created by yhd on 17.03.2020.
//

#include <iostream>
#include <opencv2/opencv.hpp>


int main() {
    cv::Mat img = imread("../data/images/lanes.jpg", cv::IMREAD_COLOR);

    cv::Mat gray;
    cvtColor(img, gray, cv::COLOR_BGR2GRAY);

    cv::Mat edges;
    Canny(gray, edges, 50, 200);

    std::vector<cv::Vec4i> lines;
    HoughLinesP(edges, lines, 1, CV_PI / 180, 100, 10, 250);
// Draw lines on the image
    for (size_t i = 0; i < lines.size(); i++) {
        cv::Vec4i line = lines[i];
        cv::line(img, cv::Point(line[0], line[1]), cv::Point(line[2], line[3]), cv::Scalar(255, 0, 0), 3, cv::LINE_AA);
    }

    imshow("Hough Lines", img);
    cv::waitKey(0);

}