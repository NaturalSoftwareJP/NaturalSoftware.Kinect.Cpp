#include "KinectSensorCollenction.h"
#include "KinectSensor.h"

namespace NaturalSoftware { namespace Kinect {

    KinectSensor& KinectSensorCollenction::operator [] ( int i )
    {
        return kinectSensors_.get()[i];
    }

    KinectSensorCollenction::KinectSensorCollenction()
    {
        // thanks:http://d.hatena.ne.jp/faith_and_brave/20110920/1316507398
        count_ = KinectSensor::GetSensorCount();
        kinectSensors_ = std::shared_ptr<KinectSensor>(
            new KinectSensor[count_], std::default_delete<KinectSensor[]>() );

        for ( int i = 0; i < count_; ++i ) {
            kinectSensors_.get()[i].CreateInstance( i );
        }
    }
}}
