//
// Created by yhd on 18.03.2020.
//

#include <opencv2/opencv.hpp>

std::string DATA_PATH = "../data/";


int main(){

    cv::Mat dst = cv::imread(DATA_PATH + "images/trump.jpg");
    cv::Mat src = cv::imread(DATA_PATH + "images/obama.jpg");

    cv::Mat src_mask = cv::imread(DATA_PATH + "images/obama-mask.jpg", cv::IMREAD_GRAYSCALE);

    imshow("Obama", src);
    imshow("Trump", dst);
    imshow("Mask", src_mask);
    cv::waitKey(0);

    // Find blob centroid
    cv::Mat src_mask_bin;
    threshold(src_mask,src_mask_bin,128,255,cv::THRESH_BINARY);

    cv::Moments m = moments(src_mask_bin);
    cv::Point center(m.m01/m.m00, m.m10/m.m00);

// Clone seamlessly
    cv::Mat output_clone;
    src.convertTo(src,CV_8UC3);
    dst.convertTo(dst,CV_8UC3);
    src_mask.convertTo(src_mask,CV_8UC3);
    seamlessClone(src,dst,src_mask,center,output_clone,cv::NORMAL_CLONE);

    imshow("Using Seamless Cloning",output_clone);
    cv::waitKey(0);

    cv::destroyAllWindows();

    return 0;
}