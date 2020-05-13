//
// Created by yhd on 17.03.2020.
//

#include <iostream>
#include <opencv2/opencv.hpp>

int main()
{
    cv::Mat img = imread("../data/images/circles.jpg", cv::IMREAD_COLOR);
// Convert to gray-scale
    cv::Mat gray;
    cvtColor(img, gray, cv::COLOR_BGR2GRAY);

// Blur the image to reduce noise
    cv::Mat img_blur;
    medianBlur(gray,img_blur,5);
// Apply hough transform on the image
    std::vector<cv::Vec3f> circles;
    HoughCircles(img_blur, circles, cv::HOUGH_GRADIENT, 1, 50, 450, 10, 30, 40);
    for( size_t i = 0; i < circles.size(); i++ )
    {
        cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        // Draw the outer circle
        circle( img, center, radius, cv::Scalar(0, 255, 0), 2);
        // Draw the center of the circle
        circle( img, center, 2, cv::Scalar(0, 0, 255), 3);
    }

    imshow("Hough Circles", img);
    cv::waitKey(0);
}