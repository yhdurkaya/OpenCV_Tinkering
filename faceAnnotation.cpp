// Created by Yusuf H. Durkaya at 23.01.2020

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

cv::Mat image, copyImage, originalImage;
cv::Point center, circumference;
bool pressed = false;

void draw(int action, int x, int y, int flags, void *userdata){
    if( action == cv::EVENT_LBUTTONDOWN )
    {
        copyImage = image.clone();
        image=originalImage.clone();
        pressed = true;
        center = cv::Point(x,y);
        cv::circle(copyImage, center, 1, cv::Scalar(255,255,0), 2, CV_AA );

    }
    else if(action == cv::EVENT_MOUSEMOVE && pressed){
        copyImage=image.clone();
        circumference.x = x;
        circumference.y = y;
        cv::rectangle(copyImage, center, circumference, cv::Scalar(0,255,0), 2, CV_AA );
        cv::imshow("Window", copyImage);

    }
    else if( action == cv::EVENT_LBUTTONUP)
    {
        pressed = false;

        circumference = cv::Point(x,y);

        cv::rectangle(image, center, circumference, cv::Scalar(0,255,0), 2, CV_AA );
        cv::imshow("Window", image);

        cv::Mat croppedImage = image(cv::Rect(center.x, center.y, circumference.x-center.x, circumference.y-center.y));
        cv::imwrite("../croppedImage.png", croppedImage);
    }

}
int main()
{
    image = cv::imread("../lena.png", cv::IMREAD_COLOR);
    cv::namedWindow("Window");
    originalImage = image.clone();
    copyImage = image.clone();

    cv::setMouseCallback("Window", draw);
    int k = 0;
    while(k!=27){
        imshow("Window", image );
        putText(image,"Choose center, and drag, Press ESC to exit and c to clear", cv::Point(10,30),
                cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255,255,255), 2 );
        k= cv::waitKey(20) & 0xFF;
    }

}
