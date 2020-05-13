//
// Created by yhd on 18.03.2020.
//


#include <string>
#include <opencv2/opencv.hpp>

std::string DATA_PATH = "../data/";

int main(){
    cv::Mat plane = cv::imread(DATA_PATH + "images/airplane.jpg");
    cv::Mat sky = cv::imread(DATA_PATH + "images/sky.jpg");

    cv::Mat mask = cv::Mat::zeros(plane.rows, plane.cols, plane.depth());

    cv::Point poly[1][7];
    poly[0][0] = cv::Point(4, 80);
    poly[0][1] = cv::Point(30, 54);
    poly[0][2] = cv::Point(151, 63);
    poly[0][3] = cv::Point(254, 37);
    poly[0][4] = cv::Point(298, 90);
    poly[0][5] = cv::Point(272, 134);
    poly[0][6] = cv::Point(43, 122);

    const cv::Point* polygons[1] = { poly[0] };
    int num_points[] = { 7 };

    fillPoly(mask, polygons, num_points, 1, cv::Scalar(255,255,255));

    cv::Point center(800,100);

    cv::Mat output;
    seamlessClone(plane, sky, mask, center, output, cv::NORMAL_CLONE);

    imshow("Output",output);
    cv::waitKey(0);

    cv::Mat img = cv::imread(DATA_PATH + "images/wood-texture.jpg");
    cv::Mat obj = cv::imread(DATA_PATH + "images/iloveyouticket.jpg");


    mask = 255 * cv::Mat::ones(obj.rows, obj.cols, obj.depth());

    center = cv::Point(img.cols/2,img.rows/2);

    cv::Mat normal_clone;
    cv::Mat mixed_clone;

    seamlessClone(obj, img, mask, center, normal_clone, cv::NORMAL_CLONE);
    seamlessClone(obj, img, mask, center, mixed_clone, cv::MIXED_CLONE);
    imshow("Normal Clone Result", normal_clone);
    imshow("Mixed Clone Result", mixed_clone);
    cv::waitKey(0);

    cv::destroyAllWindows();
}