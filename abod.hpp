
#ifndef DEF_ABOD
#define DEF_ABOD

#include <opencv2/opencv.hpp>

class Abod
{
    public:
        Abod();
        ~Abod();

        void addGround(const cv::Mat& pict);

    private:
        cv::MatND m_vhist;
        cv::MatND m_hhist;
};

#endif

