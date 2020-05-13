//
// Created by linux-pc on 11.02.2020.
//

#include <string>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int maxScaleUp = 100;
int scaleFactor = 1;
int scaleType = 0;
int maxType = 1;

std::string windowName = "Resize Image";
std::string trackbarValue = "Scale";
std::string trackbarType = "Type: \n 0: Scale Up \n 1: Scale Down";

cv::Mat image;
void resize(int, void* ){
    cv::Mat scaledImage;


    // Resize the image
    if(scaleType == 0){
        double scaleFactorDouble = 1 + scaleFactor/200.0;
        cv::resize(image, scaledImage, cv::Size(), scaleFactorDouble, scaleFactorDouble, cv::INTER_LINEAR);

    }
    else if(scaleType == 1){
        double scaleFactorDouble = 1 - scaleFactor/200.0;
        cv::resize(image, scaledImage, cv::Size(), scaleFactorDouble, scaleFactorDouble, cv::INTER_LINEAR);
    }
    imshow(windowName, scaledImage);

}
int main() {
    image = cv::imread("../lena.png", cv::IMREAD_COLOR);
    cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);

    cv::createTrackbar(trackbarValue, windowName, &scaleFactor, maxScaleUp, resize);
    cv::createTrackbar(trackbarType, windowName, &scaleType, maxType, resize);

    resize(25,0);

    while (true){
        int c;
        c = cv::waitKey(20);
        if (static_cast<char>(c) == 27)
            break;
    }
}
