#include "KinectSensor.h"
#include "SkeletonStream.h"

namespace NaturalSoftware { namespace Kinect {

    SkeletonStream::SkeletonStream( KinectSensor& kinect )
	    : kinect_( kinect )
        , smooth_( 0 )
        , flags( 0 )
        , eventHandle_( 0 )
    {
    }

    SkeletonStream::~SkeletonStream()
    {
    }

    void SkeletonStream::Enable( NUI_TRANSFORM_SMOOTH_PARAMETERS* smooth /*= 0*/ )
    {
        smooth_ = smooth;
    }

    void SkeletonStream::Disable()
    {
    }

    SkeletonFrame SkeletonStream::OpenNextFrame( DWORD timeout /*= INFINITE*/ )
    {
	    return SkeletonFrame( kinect_, *this, timeout, smooth_ );
    }

    void SkeletonStream::Start()
    {
        if ( eventHandle_ == 0 ) {
            eventHandle_ = ::CreateEvent( 0, TRUE, FALSE, 0 );
        }

        ERROR_CHECK( kinect_->NuiSkeletonTrackingEnable( eventHandle_, flags ) );
    }

    void SkeletonStream::Stop()
    {
        if ( eventHandle_ != 0 ) {
            ::CloseHandle( eventHandle_ );
            eventHandle_ = 0;
        }

        ERROR_CHECK( kinect_->NuiSkeletonTrackingDisable() );
    }

    void SkeletonStream::EnableTrackingInNearRange( bool enable )
    {
        if ( enable ) {
            flags |= NUI_SKELETON_TRACKING_FLAG_ENABLE_IN_NEAR_RANGE;
        }
        else {
            flags &= ~NUI_SKELETON_TRACKING_FLAG_ENABLE_IN_NEAR_RANGE;
        }
    }

    bool SkeletonStream::IsEnableTrackingInNearRange() const
    {
        return (flags & NUI_SKELETON_TRACKING_FLAG_ENABLE_IN_NEAR_RANGE) != 0;
    }

    void SkeletonStream::SetTrackingMode( SkeletonTrackingMode trackingMode )
    {
        if ( trackingMode == SkeletonTrackingMode::Seated ) {
            flags |= NUI_SKELETON_TRACKING_FLAG_ENABLE_SEATED_SUPPORT;
        }
        else if ( trackingMode == SkeletonTrackingMode::SkeletonTrackingMode_Default ) {
            flags &= ~NUI_SKELETON_TRACKING_FLAG_ENABLE_SEATED_SUPPORT;
        }
        else {
            throw std::invalid_argument( "invalid tracking mode." );
        }
    }

    SkeletonTrackingMode SkeletonStream::GetTrackingMode() const
    {
        if ( (flags & NUI_SKELETON_TRACKING_FLAG_ENABLE_SEATED_SUPPORT) != 0 ) {
            return SkeletonTrackingMode::Seated;
        }

        return ((flags & NUI_SKELETON_TRACKING_FLAG_ENABLE_SEATED_SUPPORT) != 0) ? 
            SkeletonTrackingMode::Seated : SkeletonTrackingMode::SkeletonTrackingMode_Default;
    }

    void SkeletonStream::AppChoosesSkeletons( bool enable )
    {
        if ( enable ) {
            flags |= NUI_SKELETON_TRACKING_FLAG_TITLE_SETS_TRACKED_SKELETONS;
        }
        else {
            flags &= ~NUI_SKELETON_TRACKING_FLAG_TITLE_SETS_TRACKED_SKELETONS;
        }
    }

    bool SkeletonStream::IsAppChoosesSkeletons() const
    {
        return (flags & NUI_SKELETON_TRACKING_FLAG_TITLE_SETS_TRACKED_SKELETONS) != 0;
    }

    bool SkeletonStream::IsSmoothingEnabled() const
    {
        return (smooth_ != 0);
    }

    NUI_TRANSFORM_SMOOTH_PARAMETERS* SkeletonStream::SmoothParameters() const
    {
        return smooth_;
    }

    void SkeletonStream::ChooseSkeletons()
    {
        ERROR_CHECK( kinect_->NuiSkeletonSetTrackedSkeletons( 0 ) );
    }

    void SkeletonStream::ChooseSkeletons( DWORD trackingId1 )
    {
        ChooseSkeletons( trackingId1, 0 );
    }

    void SkeletonStream::ChooseSkeletons( DWORD trackingId1, DWORD trackingId2 )
    {
        DWORD trackingIds[] = { trackingId1, trackingId2 };
        ERROR_CHECK( kinect_->NuiSkeletonSetTrackedSkeletons( trackingIds ) );
    }

    int SkeletonStream::GetFrameSkeletonArrayLength() const
    {
        return NUI_SKELETON_COUNT;
    }
}}
