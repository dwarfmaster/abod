#include <iostream>
#include <fstream>
#include <sstream>
#include <opencv2/opencv.hpp>
#include "abod.hpp"

int main(int argc, char *argv[])
{
    Abod abod;
    abod.load("ground");
    cv::VideoCapture input;

    if(argc == 2)
        input.open(argv[1]);
    else
        input.open(1);

    bool cont = true;
    cv::Mat img;
    while(cont) {
        input.grab();
        input.retrieve(img);
        cv::imshow("Stream", img);
        abod.compute(img);

        if((char)cvWaitKey(1) == ' ')
            cont = false;
    }

    return 0;
}

