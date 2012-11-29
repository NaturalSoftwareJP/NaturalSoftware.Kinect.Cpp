#pragma once

#include "KinectSensor.h"
#include "SkeletonFrame.h"

namespace NaturalSoftware { namespace Kinect {

    class KinectSensor;

    // enum classにしないと、DepthRange::Defaultとぶつかる
    // enum classにすると、vc100コンパイラでコンパイルできなくなる
    enum SkeletonTrackingMode
    {
        SkeletonTrackingMode_Default,
        Seated,
    };

    class SkeletonStream
    {
	    friend class KinectSensor;

    public:

	    ~SkeletonStream();

	    void Enable( NUI_TRANSFORM_SMOOTH_PARAMETERS* smooth = 0 );
        void Disable();

        SkeletonFrame OpenNextFrame( DWORD timeout = INFINITE );

        void EnableTrackingInNearRange( bool enable );
        bool IsEnableTrackingInNearRange() const;

        void SetTrackingMode( SkeletonTrackingMode trackingMode );
        SkeletonTrackingMode GetTrackingMode() const;

        void AppChoosesSkeletons( bool enable );
        bool IsAppChoosesSkeletons() const;

        bool IsSmoothingEnabled() const;

        NUI_TRANSFORM_SMOOTH_PARAMETERS* SmoothParameters() const;

        void ChooseSkeletons();
        void ChooseSkeletons( DWORD trackingId1 );
        void ChooseSkeletons( DWORD trackingId1, DWORD trackingId2 );

        int GetFrameSkeletonArrayLength() const;

    private:

	    SkeletonStream( KinectSensor& kinect );

        void Start();
        void Stop();

    private:

        KinectSensor& kinect_;

        HANDLE eventHandle_;

        NUI_TRANSFORM_SMOOTH_PARAMETERS* smooth_;

        DWORD flags;
    };
}}
