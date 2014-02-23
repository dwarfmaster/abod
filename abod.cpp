
#include "abod.hpp"
#include <iostream>
using namespace cv;

Abod::Abod()
    : m_nb(0)
{
}

Abod::~Abod()
{
}

void Abod::addGround(const cv::Mat& pict)
{
    /* Preparing picture */
    const int plotHeight = pict.size().height;
    const int plotWidth  = pict.size().width;
    cv::Mat used = pict;
    // GaussianBlur(pict, used, Size(5,5), 1.8);
    cvtColor(used, used, CV_BGR2HSV);

    /* Create mask */
    Mat mask(pict.size(), CV_8U);
    mask = Scalar(0);
    Point poly[] = { Point(20,plotHeight), Point(60,plotHeight/2), Point(plotWidth - 60,plotHeight/2), Point(plotWidth - 20,plotHeight) };
    const Point* polys[] = { poly };
    int npts[] = { 4 };
    fillPoly(mask, polys, npts, 1, Scalar(255, 255, 255));

    /* Compute hue histogram */
    int histSize[] = { 180 };
    float hranges[] = { 0, 180 };
    const float* ranges[] = { hranges };
    Mat hhist;
    int channels[] = { 0 };
    calcHist(&used, 1, channels, mask, hhist, 1, histSize, ranges, true, false);

    /* Compute saturation histogram */
    histSize[0] = 255;
    hranges[2] = 255;
    Mat shist;
    channels[0] = 1;
    calcHist(&used, 1, channels, mask, shist, 1, histSize, ranges, true, false);

    /* Debug plot */
    Mat splitted[3];
    split(used, splitted);
    imshow("Hue",        splitted[0]);
    imshow("Saturation", splitted[1]);
    imshow("Value",      splitted[2]);

    Point hpt(0,0);
    Point vpt(0,0);
    const float scale = .05f;
    for(int i = 0; i < 255; ++i) {
        Point npt(i*3, plotHeight - hhist.at<float>(i) * scale);
        if(i < 180) {
            line(used, hpt, npt, Scalar(255,0,0), 3);
            hpt = npt;
        }
        npt.y = plotHeight - shist.at<float>(i) * scale;
        line(used, vpt, npt, Scalar(0,255,0), 3);
        vpt = npt;
    }
    imshow("Hists HSV", used);

    /* Store them. */
    if(m_shist.empty())
        m_shist = shist;
    else {
        for(int i = 0; i < 255; ++i)
            m_shist.at<float>(i) = m_shist.at<float>(i) + shist.at<float>(i);
    }

    if(m_hhist.empty())
        m_hhist = hhist;
    else {
        for(int i = 0; i < 180; ++i)
            m_hhist.at<float>(i) = m_hhist.at<float>(i) + hhist.at<float>(i);
    }

    ++m_nb;
}

bool Abod::save(const std::string& path)
{
    FileStorage fs(path, FileStorage::WRITE);
    m_shist /= m_nb;
    m_hhist /= m_nb;
    fs << "sat" << m_shist;
    fs << "hue" << m_hhist;
    fs.release();
    return true;
}

bool Abod::load(const std::string& path)
{
    FileStorage fs(path, FileStorage::READ);
    fs["sat"] >> m_shist;
    fs["hue"] >> m_hhist;
    fs.release();
    return true;
}

void Abod::compute(const cv::Mat& pict)
{
    cv::Mat hsv = pict;
    // GaussianBlur(pict, hsv, Size(5,5), 1.8);
    cvtColor(hsv, hsv, CV_BGR2HSV);
    cv::Mat result;
    cvtColor(pict, result, CV_BGR2GRAY);

    /* Computing saturation histogram. */
    int histSize[] = { 180 };
    float hranges[] = { 0, 180 };
    const float* ranges[] = { hranges };
    Mat hhist;
    int channels[] = { 0 };
    calcHist(&hsv, 1, channels, Mat(), hhist, 1, histSize, ranges, true, false);

    /* Compute saturation histogram */
    histSize[0] = 255;
    hranges[2] = 255;
    Mat shist;
    channels[0] = 2;
    calcHist(&hsv, 1, channels, Mat(), shist, 1, histSize, ranges, true, false);

    for(int i = 0; i < pict.rows; ++i) {
        for(int j = 0; j < pict.cols; ++j) {
            auto vec = hsv.at<Vec<unsigned char,3>>(i,j);
            int hue = (float)vec[0];
            int sat = (float)vec[1];
            
            if(shist.at<float>(sat) < m_shist.at<float>(sat)
                    || hhist.at<float>(hue) < m_hhist.at<float>(hue))
                result.at<unsigned char>(i,j) = 0;
            else
                result.at<unsigned char>(i,j) = 255;
        }
    }
    imshow("Result", result);
}


