//
// Created by yhd on 18.03.2020.
//

#include <iostream>
#include <vector>

#include <opencv2/photo.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

std::string DATA_PATH = "../data/";

void readImagesAndTimes(std::vector<cv::Mat> &images, std::vector<float> &times){
    int numImages = 4;

    static const float timesArray[] = {1/30.0f, 0.25f, 2.5, 15.0};
    times.assign(timesArray, timesArray + numImages);

    static const char* filenames[] = {"img_0.033.jpg", "img_0.25.jpg", "img_2.5.jpg", "img_15.jpg"};

    for(int i=0; i<numImages; i++){
        cv::Mat img = cv::imread(DATA_PATH + "images/" + filenames[i]);
        images.push_back(img);
    }
}

int main() {
    std::vector<cv::Mat> images;
    std::vector<float> times;
    readImagesAndTimes(images, times);

    cv::Ptr<cv::AlignMTB> alignMTB = cv::createAlignMTB();
    alignMTB->process(images, images);

    cv::Mat responseDebevec;
    cv::Ptr<cv::CalibrateDebevec> calibrateDebevec = cv::createCalibrateDebevec();
    calibrateDebevec->process(images, responseDebevec, times);

    cv::Mat hdrDebevec;
    cv::Ptr<cv::MergeDebevec> mergeDebevec = cv::createMergeDebevec();
    mergeDebevec->process(images, hdrDebevec, times, responseDebevec);
    imwrite("hdrDebevec.hdr", hdrDebevec);

    cv::Mat ldrDrago;
    cv::Ptr<cv::TonemapDrago> tonemapDrago = cv::createTonemapDrago(1.0, 0.7);
    tonemapDrago->process(hdrDebevec, ldrDrago);
    ldrDrago = 3 * ldrDrago;

    //std::cout << ldrDrago.cols << "x" << ldrDrago.rows << std::endl;
    cv::resize(ldrDrago, ldrDrago, cv::Size(957, 720));

    imshow("Drago Tonemap",ldrDrago);
    cv::waitKey(0);

    cv::Mat ldrReinhard;
    cv::Ptr<cv::TonemapReinhard> tonemapReinhard = cv::createTonemapReinhard(1.5, 0,0,0);
    tonemapReinhard->process(hdrDebevec, ldrReinhard);

    //std::cout << ldrReinhard.cols << "x" << ldrReinhard.rows << std::endl;
    cv::resize(ldrReinhard, ldrReinhard, cv::Size(957, 720));
    imshow("Reinhard Tonemap", ldrReinhard);
    cv::waitKey(0);

    cv::Mat ldrMantiuk;
    cv::Ptr<cv::TonemapMantiuk> tonemapMantiuk = cv::createTonemapMantiuk(2.2,0.85, 1.2);
    tonemapMantiuk->process(hdrDebevec, ldrMantiuk);
    ldrMantiuk = 3 * ldrMantiuk;

    //std::cout << ldrMantiuk.cols << "x" << ldrMantiuk.rows << std::endl;
    cv::resize(ldrMantiuk, ldrMantiuk, cv::Size(957, 720));

    imshow("Mantiuk Tonemap", ldrMantiuk);
    cv::waitKey(0);
}

