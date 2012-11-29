#pragma once

#include "ImageFrame.h"

namespace NaturalSoftware { namespace Kinect {

    class DepthImageStream;
    class KinectSensor;

    class DepthImageFrame : public ImageFrame 
    {
    public:

        DepthImageFrame( KinectSensor& kinect, DepthImageStream& imageStream, DWORD timeout );
        ~DepthImageFrame();


        DWORD GetPixelDataLength() const;
        DWORD GetDepthPixelSize() const;

        const USHORT* GetPixelData() const;
        const NUI_DEPTH_IMAGE_PIXEL* GetDepthPixel() const;

        void CopyPixelDataTo( USHORT* pixelData ) const;
        void CopyDepthImagePixelDataTo( NUI_DEPTH_IMAGE_PIXEL* pixelData ) const;

    private:

        INuiFrameTexture* depthPixelFrameTexture;
        NUI_LOCKED_RECT depthPixelRect;
    };
}}
