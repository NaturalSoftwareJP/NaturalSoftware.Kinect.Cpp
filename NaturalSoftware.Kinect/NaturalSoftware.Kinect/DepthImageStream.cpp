#include "KinectSensor.h"
#include "DepthImageStream.h"

namespace NaturalSoftware { namespace Kinect {

    DepthImageStream::DepthImageStream( KinectSensor& kinect )
        : ImageStream( kinect )
        , imageType_( NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX )
    {
    }

    DepthImageStream::~DepthImageStream()
    {
        Disable();
    }

    DepthImageFormat DepthImageStream::GetFormat() const
    {
        return format_;
    }

    NUI_IMAGE_RESOLUTION DepthImageStream::GetResolution() const
    {
        return DepthImageFormatToResolution( GetFormat() );
    }

    NUI_IMAGE_TYPE DepthImageStream::GetImageType() const
    {
        return imageType_;
    }

    void DepthImageStream::Enable( DepthImageFormat format )
    {
        format_ = format;
        StartIfStarted();
    }

    void DepthImageStream::Disable()
    {
    }

    DepthImageFrame DepthImageStream::OpenNextFrame( DWORD timeout /*= INFINITE*/ )
    {
        return DepthImageFrame( kinect_, *this, timeout );
    }

    void DepthImageStream::SetRange( DepthRange range )
    {
        if ( range == DepthRange::Near ) {
            flags_ |= NUI_IMAGE_FRAME_FLAG_NEAR_MODE_ENABLED;
        }
        else if ( range == DepthRange::DepthRange_Default ) {
            flags_ &= ~NUI_IMAGE_FRAME_FLAG_NEAR_MODE_ENABLED;
        }
        else {
            throw std::invalid_argument( "invalid depth range." );
        }
    }

    DepthRange DepthImageStream::GetRange() const
    {
        return ((flags_ & NUI_IMAGE_FRAME_FLAG_NEAR_MODE_ENABLED) != 0) ?
            DepthRange::Near : DepthRange::DepthRange_Default;
    }

    NUI_IMAGE_RESOLUTION DepthImageStream::DepthImageFormatToResolution( DepthImageFormat format )
    {
        switch ( format ) {
        case Resolution640x480Fps30:
            return NUI_IMAGE_RESOLUTION_640x480;
        case Resolution320x240Fps30:
            return NUI_IMAGE_RESOLUTION_320x240;
        case Resolution80x60Fps30:
            return NUI_IMAGE_RESOLUTION_80x60;
        }

        throw std::invalid_argument( "フォーマットが不正です" );
    }

}}