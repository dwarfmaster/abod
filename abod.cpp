
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
    cv::Mat used;
    GaussianBlur(pict, used, Size(5,5), 1.8);
    cvtColor(used, used, CV_BGR2HSV);

    /* Create mask */
    Mat mask(pict.size(), CV_8U);
    const int plotHeight = pict.size().height;
    const int plotWidth  = pict.size().width;
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

    /* Compute intensity histogram */
    histSize[0] = 255;
    hranges[2] = 255;
    Mat vhist;
    channels[0] = 2;
    calcHist(&used, 1, channels, mask, vhist, 1, histSize, ranges, true, false);

    /* Debug plot */
    Point hpt(0,0);
    Point vpt(0,0);
    const float scale = .05f;
    for(int i = 0; i < 255; ++i) {
        Point npt(i*3, plotHeight - hhist.at<float>(i) * scale);
        if(i < 180) {
            line(used, hpt, npt, Scalar(255,0,0), 3);
            hpt = npt;
        }
        npt.y = plotHeight - vhist.at<float>(i) * scale;
        line(used, vpt, npt, Scalar(0,255,0), 3);
        vpt = npt;
    }
    imshow("Hists HSV", used);

    /* Store them. */
    if(m_vhist.empty())
        m_vhist = vhist;
    else {
        for(int i = 0; i < 180; ++i)
            m_vhist.at<float>(i) = m_vhist.at<float>(i) + vhist.at<float>(i);
    }

    if(m_hhist.empty())
        m_hhist = hhist;
    else {
        for(int i = 0; i < 255; ++i)
            m_hhist.at<float>(i) = m_hhist.at<float>(i) + hhist.at<float>(i);
    }

    ++m_nb;
}

bool Abod::save(const std::string& path)
{
    FileStorage fs(path, FileStorage::WRITE);
    m_vhist /= m_nb;
    m_hhist /= m_nb;
    fs << "value" << m_vhist;
    fs << "hue"   << m_hhist;
    fs.release();
    return true;
}

bool Abod::load(const std::string& path)
{
    FileStorage fs(path, FileStorage::READ);
    fs["value"] >> m_vhist;
    fs["hue"]   >> m_hhist;
    fs.release();
    return true;
}

void Abod::compute(const cv::Mat& pict)
{
    cv::Mat hsv;
    GaussianBlur(pict, hsv, Size(5,5), 1.8);
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

    /* Compute intensity histogram */
    histSize[0] = 255;
    hranges[2] = 255;
    Mat vhist;
    channels[0] = 2;
    calcHist(&hsv, 1, channels, Mat(), vhist, 1, histSize, ranges, true, false);

    for(int i = 0; i < pict.rows; ++i) {
        for(int j = 0; j < pict.cols; ++j) {
            auto vec = hsv.at<Vec<unsigned char,3>>(i,j);
            int hue   = (float)vec[0];
            int value = (float)vec[2];
            
            if(vhist.at<float>(value) < m_vhist.at<float>(value)
                    || hhist.at<float>(hue) < m_hhist.at<float>(hue))
                result.at<unsigned char>(i,j) = 0;
            else
                result.at<unsigned char>(i,j) = 255;
        }
    }
    imshow("Result", result);
}


