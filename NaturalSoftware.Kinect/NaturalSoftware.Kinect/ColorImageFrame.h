#pragma once

#include "ImageFrame.h"

namespace NaturalSoftware { namespace Kinect {

    class ColorImageStream;
    class KinectSensor;

    class ColorImageFrame : public ImageFrame 
    {
    public:

        ColorImageFrame( KinectSensor& kinect, ColorImageStream& imageStream, DWORD timeout );
        ~ColorImageFrame();

        DWORD GetPixelDataLength() const;
        const byte* GetPixelData() const;
        void CopyPixelDataTo( byte* pixelData ) const;
    };
}}
