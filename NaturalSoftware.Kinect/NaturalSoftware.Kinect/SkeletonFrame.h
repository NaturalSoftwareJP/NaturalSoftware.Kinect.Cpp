#pragma once

#include <vector>
#include "KinectSensor.h"
#include "Skeleton.h"

namespace NaturalSoftware { namespace Kinect {

    class KinectSensor;
    class SkeletonStream;

    class SkeletonFrame
    {
    public:

        typedef std::vector<Skeleton> Skeletons;

	    SkeletonFrame( KinectSensor& kinect, SkeletonStream& skeletonStream_, DWORD timeout, NUI_TRANSFORM_SMOOTH_PARAMETERS* smooth );

	    ~SkeletonFrame();

	    const Skeletons& GetSkeletonData() const
	    {
		    return skeletonData_;
	    }

    private:

        KinectSensor& kinect_;
        SkeletonStream& skeletonStream_;

        NUI_SKELETON_FRAME frame_;

	    Skeletons skeletonData_;
    };
}}
