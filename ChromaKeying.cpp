//
// Created by yhd on 21.03.2020.
//

#include <iostream>
#include <opencv2/opencv.hpp>

std::string DATA_PATH = "../data/images/";
cv::Mat frame, frameCopy;
int main()
{
    cv::VideoCapture cap(DATA_PATH + "greenscreen-asteroid.mp4");

    while(true)
    {
        cap >> frame;

        if(frame.empty())
            break;

        cv::cvtColor(frame, frameCopy, cv::COLOR_BGR2RGB);

        cv::imshow("Frame", frame);

        char c = (char) cv::waitKey(25);
        if(c==27)
            break;
    }
    cap.release();

    cv::destroyAllWindows();
}