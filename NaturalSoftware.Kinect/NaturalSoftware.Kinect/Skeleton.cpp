#include "KinectSensor.h"
#include "Skeleton.h"

namespace NaturalSoftware { namespace Kinect {

    Skeleton::Skeleton( NUI_SKELETON_DATA& skeletonData )
        : boneOrientations_( NUI_SKELETON_POSITION_COUNT )
    {
        memcpy( &skeletonData_, &skeletonData, sizeof(skeletonData_) );

        // http://msdn.microsoft.com/en-us/library/hh973073.aspx
        if ( TrackingState() == NUI_SKELETON_TRACKING_STATE::NUI_SKELETON_TRACKED ) {
            ERROR_CHECK( ::NuiSkeletonCalculateBoneOrientations( &skeletonData_, &boneOrientations_[0] ) );
        }

        joints_.reserve( NUI_SKELETON_POSITION_INDEX::NUI_SKELETON_POSITION_COUNT );
        for ( int i = 0; i < joints_.capacity(); ++i ) {
            joints_.push_back( Joint( (NUI_SKELETON_POSITION_INDEX)i,
                skeletonData_.eSkeletonPositionTrackingState[i],
                skeletonData_.SkeletonPositions[i] ) );
        }
    }

    Skeleton::~Skeleton()
    {
    }
}}
