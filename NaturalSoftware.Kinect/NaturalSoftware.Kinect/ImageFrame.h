#pragma once 

#include <Windows.h>
#include <NuiApi.h>

namespace NaturalSoftware { namespace Kinect {

    class KinectSensor;
    class ImageStream;

    class ImageFrame 
    {
    public:

        ImageFrame( KinectSensor& kinect, ImageStream& imageStream, DWORD timeout );
        ~ImageFrame();

    
        virtual DWORD GetPixelDataLength() const = 0;

        UINT GetWidth() const
        {
            return desc_.Width;
        }

        UINT GetHeight() const
        {
            return desc_.Height;
        }

        UINT GetBytesPerPixel() const
        {
            return rect_.Pitch / GetWidth();
        }

        NUI_IMAGE_RESOLUTION GetResolution() const
        {
            return frame_.eResolution;
        }

        NUI_IMAGE_TYPE GetImageType() const
        {
            return frame_.eImageType;
        }

    protected:

        KinectSensor& kinect_;
        ImageStream& imageStream_;

        NUI_IMAGE_FRAME frame_;
        NUI_LOCKED_RECT rect_;
        NUI_SURFACE_DESC desc_;
    };
}}
