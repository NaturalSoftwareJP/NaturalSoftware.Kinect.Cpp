#include "KinectSensor.h"
#include "SkeletonFrame.h"

namespace NaturalSoftware { namespace Kinect {

    SkeletonFrame::SkeletonFrame( KinectSensor& kinect, SkeletonStream& skeletonStream, DWORD timeout, NUI_TRANSFORM_SMOOTH_PARAMETERS* smooth )
	    : kinect_( kinect )
	    , skeletonStream_( skeletonStream )
    {
	    kinect_->NuiSkeletonGetNextFrame( timeout, &frame_ );
        if ( smooth != 0 ) {
            kinect_->NuiTransformSmooth( &frame_, smooth );
        }

	    skeletonData_.reserve( NUI_SKELETON_COUNT );
	    for ( int i = 0; i < NUI_SKELETON_COUNT; ++i ) {
		    skeletonData_.push_back( Skeleton( frame_.SkeletonData[i] ) );
	    }

	    //for ( auto s : frame_.SkeletonData ) {
	    //	skeletonData_.push_back( s );
	    //}
    }


    SkeletonFrame::~SkeletonFrame()
    {
    }
}}
