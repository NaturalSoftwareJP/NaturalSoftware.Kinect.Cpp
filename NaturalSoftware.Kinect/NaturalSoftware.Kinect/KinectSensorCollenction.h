#pragma once

#include <Windows.h>
#include <memory>

namespace NaturalSoftware { namespace Kinect {

class KinectSensor;

class KinectSensorCollenction
{
public:

    KinectSensorCollenction();

    template<typename Pr>
    KinectSensor& Find( Pr Pred )
    {
        auto begin = kinectSensors_.get();
        auto end = kinectSensors_.get() + count_;
        auto it = std::find_if( begin, end, Pred );
        if ( it == end ) {
            throw std::invalid_argument( "invalid pred." );
        }

        return *it;
    }

    KinectSensor& operator [] ( int i );

    UINT Count() const
    {
        return count_;
    }

private:

    // èzä¬éQè∆ÇÃÇΩÇﬂ
    std::shared_ptr<KinectSensor> kinectSensors_;
    UINT count_;
};
}}
