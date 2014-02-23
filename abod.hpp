
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

        void compute(const cv::Mat& pict, bool save = false);

    private:
        cv::MatND m_shist;
        cv::MatND m_hhist;
        float m_sthresh;
        float m_hthresh;
};

#endif

