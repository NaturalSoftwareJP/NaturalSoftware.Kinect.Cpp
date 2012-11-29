#include "ImageFrame.h"
#include "KinectSensor.h"

namespace NaturalSoftware { namespace Kinect {

    ImageFrame::ImageFrame( KinectSensor& kinect, ImageStream& imageStream, DWORD timeout )
        : kinect_( kinect )
        , imageStream_( imageStream )
    {
        ERROR_CHECK( kinect_->NuiImageStreamGetNextFrame( imageStream_.GetHandle(), timeout, &frame_ ) );
        ERROR_CHECK( frame_.pFrameTexture->LockRect( 0, &rect_, 0, 0 ) );
        ERROR_CHECK( frame_.pFrameTexture->GetLevelDesc( 0, &desc_ ) );
    }

    ImageFrame::~ImageFrame()
    {
        ERROR_CHECK( frame_.pFrameTexture->UnlockRect( 0 ) );
        ERROR_CHECK( kinect_->NuiImageStreamReleaseFrame( imageStream_.GetHandle(), &frame_ ) );
    }
}}
