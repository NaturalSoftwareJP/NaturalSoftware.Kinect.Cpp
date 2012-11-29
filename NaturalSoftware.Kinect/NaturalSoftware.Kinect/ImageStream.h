#pragma once 

#include <Windows.h>
#include <NuiApi.h>

namespace NaturalSoftware { namespace Kinect {

    class KinectSensor;

    class ImageStream
    {
    public:

        ImageStream( KinectSensor& kinect )
            : kinect_( kinect )
            , streamHandle_( 0 )
            , eventHandle_( 0 )
            , width_( 0 )
            , height_( 0 )
            , flags_( 0 )
        {
        }

        virtual ~ImageStream()
        {
            Stop();
        }

        HANDLE GetHandle() const
        {
            return streamHandle_;
        }

    
        virtual NUI_IMAGE_RESOLUTION GetResolution() const = 0;

        virtual NUI_IMAGE_TYPE GetImageType() const = 0;

        virtual DWORD GetWidth() const
        {
            return width_;
        }

        virtual DWORD GetHeight() const
        {
            return height_;
        }

    protected:

        void ResolutionToSize();
        void StartIfStarted();
    
        void Start();
        void Stop();

    protected:

        KinectSensor& kinect_;

        HANDLE streamHandle_;
        HANDLE eventHandle_;

        DWORD width_;
        DWORD height_;

        DWORD flags_;
    };
}}
