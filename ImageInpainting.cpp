//
// Created by yhd on 18.03.2020.
//

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/photo.hpp"

#include <iostream>

std::string DATA_PATH = "../data/";

cv::Mat img, inpaintMask;

cv::Mat res;
cv::Point prevPt(-1,-1);

// onMouse function for Mouse Handling
// Used to draw regions required to inpaint
static void onMouse( int event, int x, int y, int flags, void* )
{
    if( event == cv::EVENT_LBUTTONUP || !(flags & cv::EVENT_FLAG_LBUTTON) )
        prevPt = cv::Point(-1,-1);
    else if( event == cv::EVENT_LBUTTONDOWN )
        prevPt = cv::Point(x,y);
    else if( event == cv::EVENT_MOUSEMOVE && (flags & cv::EVENT_FLAG_LBUTTON) )
    {
        cv::Point pt(x,y);
        if( prevPt.x < 0 )
            prevPt = pt;
        line( inpaintMask, prevPt, pt, cv::Scalar::all(255), 5, 8, 0 );
        line( img, prevPt, pt, cv::Scalar::all(255), 5, 8, 0 );
        prevPt = pt;
        imshow("image", img);
        imshow("image: mask", inpaintMask);
    }
}

int main(int argc, char** argv){
    std::cout << "Usage: ./inpaint <image_path>" << std::endl;
    std::cout << "Keys: " << std::endl;
    std::cout << "t - inpaint using FMM" << std::endl;
    std::cout << "n - inpaint using NS technique" << std::endl;
    std::cout << "r - reset the inpainting mask" << std::endl;
    std::cout << "ESC - exit" << std::endl;

    std::string filename;
    if(argc > 1)
        filename = argv[1];
    else
        filename = "../data/images/Lincoln.jpg";

    // Read image in color mode
    img = imread(filename, cv::IMREAD_COLOR);
    cv::Mat img_mask;
    // Return error if image not read properly
    if(img.empty())
    {
        std::cout << "Failed to load image: " << filename << std::endl;
        return 0;
    }

    namedWindow("image", cv::WINDOW_AUTOSIZE);

    // Create a copy for the original image
    img_mask = img.clone();
    // Initialize mask (black image)
    inpaintMask = cv::Mat::zeros(img_mask.size(), CV_8U);

    // Show the original image
    imshow("image", img);
    cv::setMouseCallback( "image", onMouse, NULL);

    for(;;)
    {
        char c = (char)cv::waitKey();
        if (c == 't') {
            // Use Algorithm proposed by Alexendra Telea
            inpaint(img, inpaintMask, res, 3, cv::INPAINT_TELEA);
            imshow("Inpaint Output using FMM", res);
        }
        if (c == 'n') {
            // Use Algorithm proposed by Bertalmio et. al.
            inpaint(img, inpaintMask, res, 3, cv::INPAINT_NS);
            imshow("Inpaint Output using NS Technique", res);
        }
        if (c == 'r') {
            inpaintMask = cv::Scalar::all(0);
            img_mask.copyTo(img);
            imshow("image", inpaintMask);
        }
        if ( c == 27 )
            break;
    }
}