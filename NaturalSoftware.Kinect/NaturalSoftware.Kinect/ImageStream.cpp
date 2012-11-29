#include "ImageStream.h"
#include "KinectSensor.h"

namespace NaturalSoftware { namespace Kinect {

    void ImageStream::StartIfStarted()
    {
        if ( streamHandle_ != 0 ) {
            // カラーまたデプスストリームだけ止める方法がわからないので、Kinect自体を再起動する
            kinect_.Stop();
            kinect_.Start();
        }
    }

    void ImageStream::ResolutionToSize()
    {
        ::NuiImageResolutionToSize( GetResolution(), width_, height_ );
    }

    void ImageStream::Start()
    {
        if ( eventHandle_ == 0 ) {
            eventHandle_ = ::CreateEvent( 0, TRUE, FALSE, 0 );
        }

        ERROR_CHECK( kinect_->NuiImageStreamOpen( GetImageType(),
            GetResolution(), flags_, 2, eventHandle_, &streamHandle_ ) );

        ResolutionToSize();
    }

    void ImageStream::Stop()
    {
        if ( eventHandle_ != 0 ) {
            ::CloseHandle( eventHandle_ );
            eventHandle_ = 0;
        }

        streamHandle_ = 0;
    }
}}
