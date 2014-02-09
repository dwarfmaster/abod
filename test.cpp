#include <iostream>
#include <fstream>
#include <sstream>
#include <opencv2/opencv.hpp>
#include "abod.hpp"

int main()
{
    std::ifstream ifs("list_test");
    if(!ifs)
        return 1;
    std::string line;
    Abod abod;
    abod.load("ground");

    while(std::getline(ifs, line)) {
        cv::Mat img = cv::imread(line);
        imshow("Loaded", img);
        abod.compute(img);

        while((char)cvWaitKey(0) != ' ');
    }

    return 0;
}

