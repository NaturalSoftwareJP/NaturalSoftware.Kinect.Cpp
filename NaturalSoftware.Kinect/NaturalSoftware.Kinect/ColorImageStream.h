#pragma once

#include "KinectSensor.h"
#include "ColorImageFrame.h"
#include "ImageStream.h"

namespace NaturalSoftware { namespace Kinect {

    enum ColorImageFormat
    {
        ColorImageFormatUndefined = 0,
        RgbResolution640x480Fps30,
        RgbResolution1280x960Fps12,
        YuvResolution640x480Fps15,
        RawYuvResolution640x480Fps15,
        InfraredResolution640x480Fps30,
        RawBayerResolution640x480Fps30,
        RawBayerResolution1280x960Fps12,
    };

    class ColorImageStream : public ImageStream
    {
        friend class KinectSensor;

    public:

        ~ColorImageStream();

        ColorImageFormat GetFormat() const;
        NUI_IMAGE_RESOLUTION GetResolution() const;
        NUI_IMAGE_TYPE GetImageType() const;

        void Enable( ColorImageFormat format );
        void Disable();

        ColorImageFrame OpenNextFrame( DWORD timeout = INFINITE );

        static NUI_IMAGE_TYPE ColorImageFormatToImageType( ColorImageFormat format );
        static NUI_IMAGE_RESOLUTION ColorImageFormatToResolution( ColorImageFormat format );

    private:

        ColorImageStream( KinectSensor& kinect );

    private:

        ColorImageFormat format_;
    };
}}
