#pragma once

#include <Windows.h>
#include <NuiApi.h>

#include <sstream>
#include <iostream>
#include <stdexcept>


#define ERROR_CHECK( ret )                                          \
    if ( ret != S_OK ) {                                            \
        std::stringstream ss;                                       \
        ss << "failed " #ret " " << std::hex << ret << std::endl;   \
        throw std::runtime_error( ss.str().c_str() );               \
    }

#include "ImageFrame.h"
#include "ImageStream.h"
#include "ColorImageStream.h"
#include "DepthImageStream.h"
#include "SkeletonStream.h"
#include "KinectAudioSource.h"
#include "KinectSensorCollenction.h"

namespace ns = NaturalSoftware::Kinect;

namespace NaturalSoftware { namespace Kinect {

    class KinectSensor
    {
        friend KinectSensorCollenction;

    public:

	    ~KinectSensor();

        INuiSensor* operator -> () const
        {
            return kinect_;
        }

        INuiSensor* operator -> () 
        {
            return kinect_;
        }

        HRESULT Status() const
        {
            return kinect_->NuiStatus();
        }

        ColorImageStream& ColorImageStream() 
        {
            return colorImageStream_;
        }

        DepthImageStream& DepthImageStream() 
        {
            return depthImageStream_;
        }

        SkeletonStream& SkeletonStream() 
        {
            return skeletonStream_;
        }

        KinectAudioSource& AudioSource() 
        {
            return audioSource_;
        }

        void Start();
        void Stop();

        static int GetSensorCount()
        {
            int count = 0;
            ERROR_CHECK( ::NuiGetSensorCount( &count ) );
            return count;
        }

        static KinectSensorCollenction& KinectSensors();

    private:

        KinectSensor();
    
        void CreateInstance( int index )
        {
            ERROR_CHECK( ::NuiCreateSensorByIndex( index, &kinect_ ) );
        }

    private:

	    INuiSensor* kinect_;
        ns::ColorImageStream  colorImageStream_;
        ns::DepthImageStream  depthImageStream_;
        ns::SkeletonStream    skeletonStream_;
        ns::KinectAudioSource audioSource_;

        static std::shared_ptr<KinectSensorCollenction> kinectSensors_;
    };
}}
