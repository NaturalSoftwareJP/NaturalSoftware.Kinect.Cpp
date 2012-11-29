#pragma once

#include <Windows.h>
#include <NuiApi.h>

namespace NaturalSoftware { namespace Kinect {

    struct Joint
    {
    public:

        Joint()
        {
        }

        Joint( NUI_SKELETON_POSITION_INDEX type,
            NUI_SKELETON_POSITION_TRACKING_STATE state,
            Vector4 position)
            : JointType( type )
            , TrackingState( state )
            , Position( position )
        {
        }

        NUI_SKELETON_POSITION_INDEX JointType;
        NUI_SKELETON_POSITION_TRACKING_STATE TrackingState;
        Vector4 Position;
    };
}}
