
#ifndef DEF_ABOD
#define DEF_ABOD

#include <opencv2/opencv.hpp>
#include <string>

class Abod
{
    public:
        Abod();
        ~Abod();

        void addGround(const cv::Mat& pict);
        bool save(const std::string& path);
        bool load(const std::string& path);

        void compute(const cv::Mat& pict);

    private:
        cv::MatND m_vhist;
        cv::MatND m_hhist;
        unsigned int m_nb;
};

#endif

