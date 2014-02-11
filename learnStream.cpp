#include <iostream>
#include <fstream>
#include <sstream>
#include <opencv2/opencv.hpp>
#include "abod.hpp"

int main()
{
    Abod abod;
    cv::VideoCapture cam(1);

    bool cont = true;
    cv::Mat img;
    while(cont) {
        cam.grab();
        cam.retrieve(img);
        cv::imshow("Stream", img);
        abod.addGround(img);

        if((char)cvWaitKey(1) == ' ')
            cont = false;
    }
    abod.save("ground");

    return 0;
}

