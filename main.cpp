#include <iostream>
#include <fstream>
#include <sstream>
#include <opencv2/opencv.hpp>
#include "abod.hpp"

int main()
{
    std::ifstream ifs("list");
    if(!ifs)
        return 1;
    std::string line;
    Abod abod;

    while(std::getline(ifs, line)) {
        cv::Mat img = cv::imread(line);
        imshow("Loaded", img);
        abod.addGround(img);

        while((char)cvWaitKey(0) != ' ');
    }
    abod.save("ground");

    return 0;
}

