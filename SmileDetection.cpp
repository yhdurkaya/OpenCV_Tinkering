//
// Created by yhd on 5.09.2020.
//

#include <iostream>
#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define RESIZE_HEIGHT 240
#define FACE_DOWNSAMPLE_RATIO_DLIB 1

double getDistance(const dlib::vector<long,2> point1, const dlib::vector<long,2> point2){
    return sqrt(pow(point1.x() - point2.x(), 2) + pow(point1.y() - point2.y(), 2));

}

bool smile_detector(const dlib::cv_image<dlib::bgr_pixel> &cimg, \
					const dlib::rectangle &face, \
					const dlib::full_object_detection &landmarks)
{
    // Return true if a smile is detected, else return false
    bool isSmiling = false;

///////////////////////

    //auto y1 = getDistance(landmarks.part(50), landmarks.part(58));
    auto y2 = getDistance(landmarks.part(51), landmarks.part(57));
    //auto y3 = getDistance(landmarks.part(52), landmarks.part(56));
    auto d1 = getDistance(landmarks.part(48), landmarks.part(54));
    auto jaw = getDistance(landmarks.part(3), landmarks.part(13));
    std::cout << d1 / jaw << "\n";

    if( d1 /jaw > 0.485 ){
        isSmiling = true;
    }

///////////////////////

    return isSmiling;
}


int main(){

    // initialize dlib's face detector (HOG-based) and then create
    // the facial landmark predictor
    std::cout << "[INFO] loading facial landmark predictor..." << std::endl;
    dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
    dlib::shape_predictor shape_predictor;
    // Load model
    dlib::deserialize("../data/models/shape_predictor_68_face_landmarks.dat") >> shape_predictor;

    // Initializing video capture object
    cv::VideoCapture capture("../data/videos/smile.mp4");

    if (!capture.isOpened()) {
        std::cerr << "[ERROR] Unable to connect to camera" << std::endl;
    }

    // Create a VideoWriter object
    cv::VideoWriter smileDetectionOut("smileDetectionOutput.avi",
                                      cv::VideoWriter::fourcc('M','J','P','G'),
                                      15,
                                      cv::Size((int) capture.get(cv::CAP_PROP_FRAME_WIDTH),
                                               (int) capture.get(cv::CAP_PROP_FRAME_HEIGHT)));

    int frame_number = 0;
    std::vector<int> smile_frames;

    cv::Mat frame, frame_small;
    float IMAGE_RESIZE;

    while(capture.read(frame)){

        if (frame.empty()) {
            std::cout << "[ERROR] Unable to capture frame" << std::endl;
            break;
        }

        //std::cout << "Processing frame: " << frame_number << std::endl;

        IMAGE_RESIZE = (float)frame.rows / RESIZE_HEIGHT;
        cv::resize(frame, frame, cv::Size(), 1.0 / IMAGE_RESIZE, 1.0 / IMAGE_RESIZE);
        cv::resize(frame, frame_small, cv::Size(), 1.0 / FACE_DOWNSAMPLE_RATIO_DLIB, 1.0 / FACE_DOWNSAMPLE_RATIO_DLIB);

        // Turn OpenCV's Mat into something dlib can deal with. Note that this just
        // wraps the Mat object, it doesn't copy anything. So cimg is only valid as
        // long as frame is valid.  Also don't do anything to frame that would cause it
        // to reallocate the memory which stores the image as that will make cimg
        // contain dangling pointers.  This basically means you shouldn't modify frame
        // while using cimg.
        dlib::cv_image<dlib::bgr_pixel> cimg(frame);
        dlib::cv_image<dlib::bgr_pixel> cimg_small(frame_small);

        // Detect faces
        std::vector<dlib::rectangle> faces = detector(cimg_small);

        // if # faces detected is zero
        if (0 == faces.size()) {
            putText(frame, "Unable to detect face, Please check proper lighting", cv::Point(10, 30), cv::FONT_HERSHEY_COMPLEX, 0.3, cv::Scalar(0, 0, 255), 1, cv::LINE_AA);
            putText(frame, "Or Decrease FACE_DOWNSAMPLE_RATIO", cv::Point(10, 50), cv::FONT_HERSHEY_COMPLEX, 0.3, cv::Scalar(0, 0, 255), 1, cv::LINE_AA);
        }
        else {
            dlib::rectangle face(
                    (long)(faces[0].left() * FACE_DOWNSAMPLE_RATIO_DLIB),
                    (long)(faces[0].top() * FACE_DOWNSAMPLE_RATIO_DLIB),
                    (long)(faces[0].right() * FACE_DOWNSAMPLE_RATIO_DLIB),
                    (long)(faces[0].bottom() * FACE_DOWNSAMPLE_RATIO_DLIB)
            );
            dlib::full_object_detection landmarks = shape_predictor(cimg, face);
            if (smile_detector(cimg, face, landmarks)) {
                cv::putText(frame, cv::format("Smiling :)"), cv::Point(10, 30),
                            cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
                smile_frames.push_back(frame_number);
                cv::line(frame, cv::Point(landmarks.part(48).x(), landmarks.part(48).y()), cv::Point(landmarks.part(54).x(), landmarks.part(54).y()), (0, 0, 255), 1);
                //cv::line(frame, cv::Point(landmarks.part(50).x(), landmarks.part(50).y()), cv::Point(landmarks.part(58).x(), landmarks.part(58).y()), (0, 255, 0), 1);
                //cv::line(frame, cv::Point(landmarks.part(51).x(), landmarks.part(51).y()), cv::Point(landmarks.part(57).x(), landmarks.part(57).y()), (0, 255, 0), 1);
                //cv::line(frame, cv::Point(landmarks.part(52).x(), landmarks.part(52).y()), cv::Point(landmarks.part(56).x(), landmarks.part(56).y()), (0, 255, 0), 1);
                cv::line(frame, cv::Point(landmarks.part(3).x(), landmarks.part(3).y()), cv::Point(landmarks.part(13).x(), landmarks.part(13).y()), (0, 255, 0), 1);
            }
            auto dist = getDistance(landmarks.part(48), landmarks.part(54)) / getDistance(landmarks.part(3), landmarks.part(13));

            cv::putText(frame, std::to_string(dist), cv::Point(30, 60),
                        cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
        }


        if (frame_number%50 == 0){
            std::cout << "\nProcessed " << frame_number << " frames" << std::endl;
            std::cout << "Smile detected in " << smile_frames.size() << " number of frames" << std::endl;
        }
        // Write to VideoWriter
        cv::resize(frame, frame, cv::Size(), IMAGE_RESIZE, IMAGE_RESIZE);
        smileDetectionOut.write(frame);
        frame_number++;
    }

    capture.release();
    smileDetectionOut.release();

    return 0;
}