#include <iostream>
#include <fstream>
#include <sstream>
#include <opencv2/opencv.hpp>
#include "abod.hpp"

int main()
{
    Abod abod;
    abod.load("ground");
    cv::VideoCapture cam(1);

    bool cont = true;
    cv::Mat img;
    while(cont) {
        cam.grab();
        cam.retrieve(img);
        cv::imshow("Stream", img);
        abod.compute(img);

        if((char)cvWaitKey(1) == ' ')
            cont = false;
    }

    return 0;
}

