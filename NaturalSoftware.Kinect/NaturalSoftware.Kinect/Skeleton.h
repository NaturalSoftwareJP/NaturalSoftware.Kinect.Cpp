#pragma once

#include <Windows.h>
#include <NuiApi.h>
#include <vector>

#include "Joint.h"

namespace NaturalSoftware { namespace Kinect {

    enum FrameEdges
    {
        None   = 0x0,
        Right  = 0x1,
        Left   = 0x2,
        Top    = 0x4,
        Bottom = 0x8,
    };

    class Skeleton
    {
    public:

        typedef std::vector<NUI_SKELETON_BONE_ORIENTATION> BoneOrientationCollection;
        typedef std::vector<Joint> JointCollection;

        explicit Skeleton( NUI_SKELETON_DATA& skeletonData );
        ~Skeleton();

        NUI_SKELETON_TRACKING_STATE TrackingState() const
        {
            return skeletonData_.eTrackingState;
        }

        DWORD TrackingId() const
        {
            return skeletonData_.dwTrackingID;
        }

        DWORD UserIndex() const
        {
            return skeletonData_.dwUserIndex;
        }

        const Vector4& Position() const
        {
            return skeletonData_.Position;
        }

        FrameEdges ClippedEdges() const
        {
            return (FrameEdges)skeletonData_.dwQualityFlags;
        }

        DWORD EnrollmentIndex() const
        {
            return skeletonData_.dwEnrollmentIndex;
        }

        const BoneOrientationCollection& GetBoneOrientations() const
        {
            return boneOrientations_;
        }

        const JointCollection& GetJoints() const
        {
            return joints_;
        }

        const NUI_SKELETON_DATA* GetSkeletonData() const
        {
            return &skeletonData_;
        }

    private:

        NUI_SKELETON_DATA skeletonData_;
        BoneOrientationCollection boneOrientations_;
        JointCollection joints_;
    };
}}
