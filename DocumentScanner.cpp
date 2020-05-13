//
// Created by yhd on 5.05.2020.
//

#include <string>

#include <opencv2/opencv.hpp>

std::string DATA_PATH = "../data/images/";

int main(){
    cv::Mat sourceImage = cv::imread(DATA_PATH + "scanned-form.jpg");
    std::cout << sourceImage.size() << std::endl;

    cv::resize(sourceImage, sourceImage, cv::Size(500, 666.5));

    cv::Mat sourceGray, detectedEdges;
    cv::cvtColor(sourceImage, sourceGray, cv::COLOR_BGR2GRAY);
    cv::blur(sourceGray, detectedEdges, cv::Size(3, 3));
    cv::Canny(detectedEdges, detectedEdges, 150, 200, 3);
    //cv::imshow("Detected Edges", detectedEdges);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(detectedEdges, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    cv::Mat drawing = cv::Mat::zeros(detectedEdges.size(), CV_8UC3);

    double maxArea = 0;
    int maxContourAreaID = -1;

    std::vector<std::vector<cv::Point>> contoursPoly(contours.size());
    std::vector<cv::Rect> boundRect(contours.size());

    for( size_t i = 0; i< contours.size(); i++ ){
        cv::Scalar color = cv::Scalar(0, 255, 0 );
        double newArea = cv::contourArea(contours.at(i));

        cv::approxPolyDP(contours[i], contoursPoly[i], 3, true);
        boundRect[i] = cv::boundingRect(contoursPoly[i]);

        if(newArea > maxArea) {
            maxArea = newArea;
            maxContourAreaID = i;
        }
    }

    //drawContours( sourceImage, contoursPoly, (int) maxContourAreaID, cv::Scalar(0, 255, 0), 2);
    //rectangle( sourceImage, boundRect[maxContourAreaID].tl(), boundRect[maxContourAreaID].br(), cv::Scalar(0, 0, 255), 2 );
    //circle( sourceImage, centers[maxContourAreaID], (int)radius[maxContourAreaID], cv::Scalar(0, 255, 0), 2 );

    std::vector<cv::Point2f> sourcePoints{contoursPoly[maxContourAreaID][1], contoursPoly[maxContourAreaID][0], contoursPoly[maxContourAreaID][2], contoursPoly[maxContourAreaID][3]};
    std::vector<cv::Point2f> destinationPoints;

    destinationPoints.push_back(cv::Point2f(boundRect[maxContourAreaID].x, boundRect[maxContourAreaID].y));
    destinationPoints.push_back(cv::Point2f((boundRect[maxContourAreaID].x + boundRect[maxContourAreaID].width), (boundRect[maxContourAreaID].y)));
    destinationPoints.push_back(cv::Point2f((boundRect[maxContourAreaID].x), (boundRect[maxContourAreaID].y + boundRect[maxContourAreaID].height)));
    destinationPoints.push_back(cv::Point2f((boundRect[maxContourAreaID].x + boundRect[maxContourAreaID].width), (boundRect[maxContourAreaID].y + boundRect[maxContourAreaID].height)));

    cv::Mat homography = cv::findHomography(sourcePoints, destinationPoints);

    cv::Mat scannedImage;
    cv::warpPerspective(sourceImage, scannedImage, homography, cv::Size(sourceImage.size()));

    //cv::imshow("Contours", sourceImage);
    cv::imshow("Scanned Image", scannedImage);

    std::cout << contoursPoly[maxContourAreaID] << std::endl;

    cv::waitKey(0);

}