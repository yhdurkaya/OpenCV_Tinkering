//
// Created by yhd on 9.05.2020.
//

#include <string>
#include <unistd.h>

#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/dnn.hpp>

std::string DATA_PATH = "../data/videos/";
std::string MODEL_PATH = "../data/models/";

#define SSTR( x ) static_cast< std::ostringstream & >( ( std::ostringstream() << std::dec << x ) ).str()

// Initialize the parameters
float objectnessThreshold = 0.5; // Objectness threshold
float confThreshold = 0.5; // Confidence threshold
float nmsThreshold = 0.4;  // Non-maximum suppression threshold
int inpWidth = 416;  // Width of network's input image
int inpHeight = 416; // Height of network's input image

std::vector<std::string> classes;
std::vector<cv::Rect> boxes;

std::string trackerType = "KCF";

auto getOutputsNames(const cv::dnn::Net& net)
{
    static std::vector<cv::String> names;
    if (names.empty())
    {
        //Get the indices of the output layers, i.e. the layers with unconnected outputs
        std::vector<int> outLayers = net.getUnconnectedOutLayers();

        //get the names of all the layers in the network
        std::vector<cv::String> layersNames = net.getLayerNames();

        // Get the names of the output layers in names
        names.resize(outLayers.size());
        for (size_t i = 0; i < outLayers.size(); ++i)
            names[i] = layersNames[outLayers[i] - 1];
    }
    return names;
}

void drawPred(int classId, float conf, int left, int top, int right, int bottom, cv::Mat& frame)
{
    //Draw a rectangle displaying the bounding box
    rectangle(frame, cv::Point(left, top), cv::Point(right, bottom), cv::Scalar(255, 178, 50), 3);

    //Get the label for the class name and its confidence
    std::string label = cv::format("%.2f", conf);

    if (!classes.empty())
    {
        CV_Assert(classId < (int)classes.size());
        label = classes[classId] + ":" + label;
    }

    //Display the label at the top of the bounding box
    int baseLine;
    cv::Size labelSize = getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
    top = cv::max(top, labelSize.height);
    rectangle(frame, cv::Point(left, top - round(1.5*labelSize.height)), cv::Point(left + round(1.5*labelSize.width), top + baseLine), cv::Scalar(255, 255, 255), cv::FILLED);
    putText(frame, label, cv::Point(left, top), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0,0,0),1);
}

void postprocess(cv::Mat& frame, const std::vector<cv::Mat>& outs)
{
    std::vector<int> classIds;
    std::vector<float> confidences;

    for (size_t i = 0; i < outs.size(); ++i)
    {
        // Scan through all the bounding boxes output from the network and keep only the
        // ones with high confidence scores. Assign the box's class label as the class
        // with the highest score for the box.
        float* data = (float*)outs[i].data;
        for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
        {
            cv::Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
            cv::Point classIdPoint;
            double confidence;
            // Get the value and location of the maximum score
            minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
            if (confidence > confThreshold)
            {
                int centerX = (int)(data[0] * frame.cols);
                int centerY = (int)(data[1] * frame.rows);
                int width = (int)(data[2] * frame.cols);
                int height = (int)(data[3] * frame.rows);
                int left = centerX - width / 2;
                int top = centerY - height / 2;

                if(classIdPoint.x == 32) {
                    classIds.push_back(classIdPoint.x);
                    confidences.push_back((float) confidence);
                    boxes.push_back(cv::Rect(left, top, width, height));
                    std::cout << boxes.size() << "xxx" << confidences.size() << " ----- ";
                }
            }
        }
    }

    // Perform non maximum suppression to eliminate redundant overlapping boxes with
    // lower confidences
    std::vector<int> indices;
    std::cout << boxes.size() << "  x  " << confidences.size() << "\n";
    cv::dnn::NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);

    for (size_t i = 0; i < indices.size(); ++i)
    {
        int idx = indices[i];

        cv::Rect box = boxes[idx];

        drawPred(classIds[idx], confidences[idx], box.x, box.y,
                box.x + box.width, box.y + box.height, frame);

    }

}

int main(){

    std::string classesFile = MODEL_PATH + "coco.names";
    std::ifstream ifs(classesFile.c_str());
    std::string line;
    while (getline(ifs, line)) classes.push_back(line);

    cv::String modelConfiguration = MODEL_PATH + "yolov3.cfg";
    cv::String modelWeights = MODEL_PATH + "yolov3.weights";

    // Load the network
    cv::dnn::Net net = cv::dnn::readNetFromDarknet(modelConfiguration, modelWeights);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

    cv::Ptr<cv::Tracker> tracker;
    tracker = cv::TrackerKCF::create();

    std::string videoPath = DATA_PATH + "soccer-ball.mp4";

    cv::VideoCapture cap(videoPath);
    cv::VideoWriter video;

    if(!cap.isOpened()){
        std::cout << "Could not read the video file!" << "\n";
    }
    std::string outputFile = "balltracking.cpp";
    video.open(outputFile, cv::VideoWriter::fourcc('M','J','P','G'), 28, cv::Size(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT)));

    static const std::string kWinName = "Detection and Tracking";
    cv::namedWindow(kWinName, cv::WINDOW_FULLSCREEN);

    cv::Mat frame;
    bool ok = cap.read(frame);

    cv::Mat blob;
    cv::dnn::blobFromImage(frame, blob, 1 / 255.0, cv::Size(inpWidth, inpHeight), cv::Scalar(0, 0, 0), true, false);

    net.setInput(blob);

    std::vector<cv::Mat> outs;
    net.forward(outs, getOutputsNames(net));

    postprocess(frame, outs);

    std::vector<double> layersTimes;
    double freq = cv::getTickFrequency() / 1000;
    double t = net.getPerfProfile(layersTimes) / freq;
    std::string label = cv::format("Inference time for a frame : %.2f ms", t);
    putText(frame, label, cv::Point(0, 15), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));

    cv::imshow(kWinName, frame);
    cv::waitKey(2000);

    cv::Rect2d boundingBox(boxes[0]);
    boxes.clear();

    tracker->init(frame, boundingBox);

    size_t failCount = 0;

    while(cap.read(frame)) {
        double timer = (double) cv::getTickCount();

        // Update the tracking result
        bool success = tracker->update(frame, boundingBox);

        // Calculate Frames per second (FPS)
        float fps = cv::getTickFrequency() / ((double) cv::getTickCount() - timer);

        if(success){
            cv::rectangle(frame, boundingBox, cv::Scalar(0, 255, 0), 2, 1);
        }
        else{
            putText(frame, "Tracking failure detected", cv::Point(100,80), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0,0,255),2);

            failCount++;

            if(failCount == 25){
                failCount = 0;

                tracker->clear();
                cv::Ptr<cv::Tracker> trackerNew = cv::TrackerKCF::create();
                tracker = trackerNew;

                cv::dnn::blobFromImage(frame, blob, 1 / 255.0, cv::Size(inpWidth, inpHeight), cv::Scalar(0, 0, 0), true, false);

                net.setInput(blob);

                std::vector<cv::Mat> out;
                net.forward(out, getOutputsNames(net));

                postprocess(frame, out);

                //std::vector<double> layersTimes;
                //double freq = cv::getTickFrequency() / 1000;
                //double t = net.getPerfProfile(layersTimes) / freq;
                //std::string label = cv::format("Inference time for a frame : %.2f ms", t);
                putText(frame, label, cv::Point(0, 15), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255));

                boundingBox = boxes.back();

                tracker->init(frame, boundingBox);

                if(!boxes.empty()) {
                    boxes.clear();
                }
            }
        }

        putText(frame, trackerType + " Tracker", cv::Point(100,20), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(50,170,50),2);

        // Display FPS on frame
        putText(frame, "FPS : " + SSTR(int(fps)), cv::Point(100,50), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(50,170,50), 2);

        cv::imshow(kWinName, frame);

        char c = (char) cv::waitKey(25);
        if(c == 27){
            break;
        }
    }
    cap.release();
    cv::destroyAllWindows();
}