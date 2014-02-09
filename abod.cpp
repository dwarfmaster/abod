
#include "abod.hpp"
#include <iostream>
using namespace cv;

Abod::Abod()
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
    Mat mask(480, 640, CV_8U);
    mask = Scalar(0);
    Point poly[] = { Point(20,480), Point(60,200), Point(580,200), Point(620,480) };
    const Point* polys[] = { poly };
    int npts[] = { 4 };
    fillPoly(mask, polys, npts, 1, Scalar(255, 255, 255));

    /* Compute hue histogram */
    int histSize[] = { 30 };
    float hranges[] = { 0, 180 };
    const float* ranges[] = { hranges };
    MatND hhist;
    int channels[] = { 0 };
    calcHist(&used, 1, channels, Mat(), hhist, 1, histSize, ranges, true, false);

    /* Compute intensity histogram */
    histSize[0] = 30;
    hranges[2] = 255;
    MatND vhist;
    channels[0] = 2;
    calcHist(&used, 1, channels, mask, vhist, 1, histSize, ranges, true, false);

    /* Debug plot */
    Point hpt(0,0);
    Point vpt(0,0);
    const float scale = .05f;
    for(int i = 0; i < 30; ++i) {
        Point npt(i*21, hhist.at<float>(i) * scale);
        line(used, hpt, npt, Scalar(255,0,0), 3);
        hpt = npt;
        npt.y = vhist.at<float>(i) * scale;
        line(used, vpt, npt, Scalar(0,255,0), 3);
        vpt = npt;
    }
    imshow("Hists HSV", used);

    /* Store them. */
    if(m_vhist.empty())
        m_vhist = vhist;
    else {
        for(int i = 0; i < 30; ++i)
            m_vhist.at<unsigned int>(i, m_vhist.at<unsigned int>(i) | vhist.at<unsigned int>(i));
    }

    if(m_hhist.empty())
        m_hhist = hhist;
    else {
        for(int i = 0; i < 30; ++i)
            m_hhist.at<unsigned int>(i, m_hhist.at<unsigned int>(i) | hhist.at<unsigned int>(i));
    }
}

bool Abod::save(const std::string& path)
{
    FileStorage fs(path, FileStorage::WRITE);
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

    m_vthresh = m_hthresh = 0;
    for(int i = 0; i < 30; ++i) {
        m_vthresh += m_vhist.at<unsigned int>(i);
        m_hthresh += m_hhist.at<unsigned int>(i);
    }
    m_vthresh /= 30;
    m_hthresh /= 30;
    return true;
}

void Abod::compute(const cv::Mat& pict)
{
    cv::Mat hsv;
    cvtColor(pict, hsv, CV_BGR2HSV);
    cv::Mat result;
    cvtColor(pict, result, CV_BGR2GRAY);

    for(int r = 0; r < pict.size().height; ++r) {
        for(int c = 0; c < pict.size().width; ++c) {
            int hue   = hsv.ptr<float>(c)[3*r];
            hue = (float)hue / 180.0f * 30.0f;
            int value = hsv.ptr<float>(c)[3*r+2];
            value = (float)value / 255.0f * 30.0f;
            
            if(m_vhist.at<int>(value) < m_vthresh || m_hhist.at<int>(value) < m_hthresh)
                result.at<char>(c, r, 0);
            else
                result.at<char>(c, r, 255);
        }
    }
    imshow("Result", result);
}


