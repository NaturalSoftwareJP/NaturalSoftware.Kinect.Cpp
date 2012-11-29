#pragma once

#include "KinectSensor.h"
#include "DepthImageFrame.h"
#include "ImageStream.h"

namespace NaturalSoftware { namespace Kinect {

    enum DepthImageFormat
    {
        DepthImageFormatUndefined = 0,
        Resolution640x480Fps30,
        Resolution320x240Fps30,
        Resolution80x60Fps30,
    };

    // enum classにしないと、SkeletonTrackingMode::Defaultとぶつかる
    // enum classにすると、vc100コンパイラでコンパイルできなくなる
    enum DepthRange
    {
        DepthRange_Default,
        Near,
    };

    class DepthImageStream : public ImageStream
    {
        friend class KinectSensor;

    public:

        ~DepthImageStream();

        DepthImageFormat GetFormat() const;
        NUI_IMAGE_RESOLUTION GetResolution() const;
        NUI_IMAGE_TYPE GetImageType() const;

        void Enable( DepthImageFormat format );
        void Disable();

        DepthImageFrame OpenNextFrame( DWORD timeout = INFINITE );

        void SetRange( DepthRange range );
        DepthRange GetRange() const;

        static NUI_IMAGE_RESOLUTION DepthImageFormatToResolution( DepthImageFormat format );

    private:

        DepthImageStream( KinectSensor& kinect );

    private:

        DepthImageFormat format_;

        NUI_IMAGE_TYPE imageType_;
    };
}}
