#include "KinectSensor.h"
#include "ImageFrame.h"

#include <sstream>
#include <iostream>

namespace NaturalSoftware { namespace Kinect {

    /*static*/ std::shared_ptr<KinectSensorCollenction> KinectSensor::kinectSensors_;

    KinectSensor::KinectSensor()
        : colorImageStream_( *this )
        , depthImageStream_( *this )
	    , skeletonStream_( *this )
        , audioSource_( *this )
    {
    }


    KinectSensor::~KinectSensor()
    {
        Stop();
    }

    void KinectSensor::Start()
    {
	    ERROR_CHECK( kinect_->NuiInitialize(
		    NUI_INITIALIZE_FLAG_USES_COLOR |
		    NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX |
		    NUI_INITIALIZE_FLAG_USES_SKELETON |
		    NUI_INITIALIZE_FLAG_USES_AUDIO ) );

        colorImageStream_.Start();
        depthImageStream_.Start();
        skeletonStream_.Start();
    }

    void KinectSensor::Stop()
    {
        colorImageStream_.Stop();
        depthImageStream_.Stop();
        skeletonStream_.Stop();

        kinect_->NuiShutdown();
    }

    /*static*/ KinectSensorCollenction& KinectSensor::KinectSensors()
    {
        if ( kinectSensors_.get() == 0 ) {
            kinectSensors_ = std::shared_ptr<KinectSensorCollenction>( new KinectSensorCollenction() );
        }

        return *kinectSensors_;
    }
}}
