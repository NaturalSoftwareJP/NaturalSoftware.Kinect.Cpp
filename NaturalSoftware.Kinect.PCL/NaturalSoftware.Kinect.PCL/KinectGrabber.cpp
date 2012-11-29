#include "KinectGrabber.h"

namespace pcl {
    KinectGrabber::KinectGrabber()
        : Grabber()
        , isRunning_( false )
        , quit_( false )
    {
        point_cloud_rgba_signal_  = createSignal<sig_cb_openni_point_cloud_rgba> ();
    }

    /*virtual*/ void KinectGrabber::start()
    {
        kinect_.CreateInstance( 0 );
        kinect_.ColorImageStream().Enable( ns::ColorImageFormat::RgbResolution640x480Fps30 );
        kinect_.DepthImageStream().Enable( ns::DepthImageFormat::Resolution640x480Fps30 );
        kinect_.Start();

        boost::lock_guard<boost::mutex> image_lock (image_mutex_);
        image_thread_ = boost::thread (&KinectGrabber::ImageDataThreadFunction, this);
    }

    /*virtual*/ void KinectGrabber::stop()
    {
        quit_ = true;
    }

    /*virtual*/ bool KinectGrabber::isRunning() const
    {
        return isRunning_;
    }

    /*virtual*/ std::string KinectGrabber::getName() const
    {
        return "KinectGrabber";
    }

    /*virtual*/ float KinectGrabber::getFramesPerSecond() const
    {
        return 30;
    }

    void KinectGrabber::ImageDataThreadFunction()
    {
        while (true)
        {
            // lock before checking running flag
            boost::unique_lock<boost::mutex> image_lock (image_mutex_);
            if ( quit_ ) {
                return;
            }

            //image_condition_.wait (image_lock);
            //if (quit_)
            //    return;

            auto colorFrame = kinect_.ColorImageStream().OpenNextFrame();
            auto depthFrame = kinect_.DepthImageStream().OpenNextFrame();
            image_lock.unlock ();

            if (point_cloud_rgba_signal_->num_slots () > 0) {
                point_cloud_rgba_signal_->operator()(convertToXYZRGBAPointCloud( colorFrame, depthFrame ) );
            }
        }
    }

#pragma pack( push, 1 )
    struct BGRX
    {
        byte b;
        byte g;
        byte r;
        byte x;
    };
#pragma pack( pop )

    /*static*/ PointCloud<pcl::PointXYZRGB>::Ptr KinectGrabber::convertToXYZRGBAPointCloud(
        ns::ColorImageFrame& colorFrame, const ns::DepthImageFrame& depthFrame )
    {
        // ポイントクラウド準備
        pcl::PointCloud<pcl::PointXYZRGB>::Ptr points(new pcl::PointCloud<pcl::PointXYZRGB>);

        const BGRX* colorPixel = (BGRX*)colorFrame.GetPixelData();
        const USHORT* depth = depthFrame.GetPixelData();

        int size = depthFrame.GetPixelDataLength() / sizeof(USHORT);

        for ( int i = 0; i < size; ++i ) {
            pcl::PointXYZRGB point;

            USHORT distance = ::NuiDepthPixelToDepth( depth[i] );
            LONG depthX = i % colorFrame.GetWidth();
            LONG depthY = i / colorFrame.GetWidth();

            // ポイントクラウド
            Vector4 real = ::NuiTransformDepthImageToSkeleton(
                depthX, depthY, distance, depthFrame.GetResolution() );
            point.x = real.x;
            point.y = -real.y;
            point.z = real.z;

            // 距離カメラの座標を、RGBカメラの座標に変換する
            LONG colorX = depthX;
            LONG colorY = depthY;
            ::NuiImageGetColorPixelCoordinatesFromDepthPixelAtResolution(
                colorFrame.GetResolution(), depthFrame.GetResolution(),
                0, depthX , depthY, 0, &colorX, &colorY );

            // テクスチャ
            const BGRX& pixel = colorPixel[(colorY * colorFrame.GetWidth()) + colorX];
            point.r = pixel.r;
            point.g = pixel.g;
            point.b = pixel.b;

            points->push_back(point);
        }

        return points;
    }
}