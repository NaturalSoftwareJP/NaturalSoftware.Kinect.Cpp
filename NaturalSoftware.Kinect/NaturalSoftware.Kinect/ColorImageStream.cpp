#include "KinectSensor.h"
#include "ColorImageStream.h"

namespace NaturalSoftware { namespace Kinect {

    ColorImageStream::ColorImageStream( KinectSensor& kinect )
        : ImageStream( kinect )
    {
    }

    ColorImageStream::~ColorImageStream()
    {
        Stop();
    }

    ColorImageFormat ColorImageStream::GetFormat() const
    {
        return format_;
    }

    NUI_IMAGE_RESOLUTION ColorImageStream::GetResolution() const
    {
        return ColorImageFormatToResolution( GetFormat() );
    }

    NUI_IMAGE_TYPE ColorImageStream::GetImageType() const
    {
        return ColorImageFormatToImageType( GetFormat() );
    }

    void ColorImageStream::Enable( ColorImageFormat format )
    {
        format_ = format;
        StartIfStarted();
    }

    void ColorImageStream::Disable()
    {
    }

    ColorImageFrame ColorImageStream::OpenNextFrame( DWORD timeout /*= INFINITE*/ )
    {
        return ColorImageFrame( kinect_, *this, timeout );
    }


    NUI_IMAGE_TYPE ColorImageStream::ColorImageFormatToImageType( ColorImageFormat format )
    {
        switch ( format ) {
        case RgbResolution640x480Fps30:
        case RgbResolution1280x960Fps12:
            return NUI_IMAGE_TYPE_COLOR;
        case YuvResolution640x480Fps15:
            return NUI_IMAGE_TYPE_COLOR_YUV;
        case RawYuvResolution640x480Fps15:
            return NUI_IMAGE_TYPE_COLOR_RAW_YUV;
        case InfraredResolution640x480Fps30:
            return NUI_IMAGE_TYPE_COLOR_INFRARED;
        case RawBayerResolution640x480Fps30:
        case RawBayerResolution1280x960Fps12:
            return NUI_IMAGE_TYPE_COLOR_RAW_BAYER;
        }

        throw std::invalid_argument( "フォーマットが不正です" );
    }

    NUI_IMAGE_RESOLUTION ColorImageStream::ColorImageFormatToResolution( ColorImageFormat format )
    {
        switch ( format ) {
        case RgbResolution640x480Fps30:
        case YuvResolution640x480Fps15:
        case RawYuvResolution640x480Fps15:
        case InfraredResolution640x480Fps30:
        case RawBayerResolution640x480Fps30:
            return NUI_IMAGE_RESOLUTION_640x480;
        case RgbResolution1280x960Fps12:
        case RawBayerResolution1280x960Fps12:
            return NUI_IMAGE_RESOLUTION_1280x960;
        }

        throw std::invalid_argument( "フォーマットが不正です" );
    }
}}
