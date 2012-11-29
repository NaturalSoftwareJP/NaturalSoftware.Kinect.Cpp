#pragma once

#include <NaturalSoftware.Kinect\NaturalSoftware.Kinect\KinectSensor.h>

#undef min
#undef max

#include <pcl/io/grabber.h>
#include <string>
#include <deque>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>

#include <pcl/common/synchronizer.h>
#include <pcl/point_types.h>

namespace pcl
{
    struct PointXYZ;
    struct PointXYZRGB;
    struct PointXYZRGB;
    struct PointXYZI;
    template <typename T> class PointCloud;

    class KinectGrabber : public Grabber
    {
    private:

        typedef void (sig_cb_openni_point_cloud_rgba) (const boost::shared_ptr<const pcl::PointCloud<pcl::PointXYZRGB> >&);


        bool isRunning_;

        Synchronizer<boost::shared_ptr<ns::ColorImageFrame>, boost::shared_ptr<ns::DepthImageFrame> > rgb_sync_;

        boost::signals2::signal<sig_cb_openni_point_cloud_rgba>* point_cloud_rgba_signal_;

        ns::KinectSensor kinect_;

        bool quit_;
        mutable boost::mutex image_mutex_;
        mutable boost::mutex depth_mutex_;
        boost::condition_variable image_condition_;
        boost::condition_variable depth_condition_;
        boost::thread image_thread_;
        boost::thread depth_thread_;

    public:

        KinectGrabber();

        virtual void start();
        virtual void stop();

        virtual bool isRunning() const;
        
        virtual std::string getName() const;
        virtual float getFramesPerSecond() const;

        static PointCloud<pcl::PointXYZRGB>::Ptr convertToXYZRGBAPointCloud(
            ns::ColorImageFrame& colorFrame, const ns::DepthImageFrame& depthFrame );

    private:

        void ImageDataThreadFunction();
    };
}

