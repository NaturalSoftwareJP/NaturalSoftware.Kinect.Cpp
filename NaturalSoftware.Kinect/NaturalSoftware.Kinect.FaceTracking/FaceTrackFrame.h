#pragma once

#include <FaceTrackLib.h>

namespace NaturalSoftware { namespace Kinect { namespace FaceTracking {

    class FaceTracker;

    class FaceTrackFrame
    {
        friend class FaceTracker;

    public:

        const std::vector<FT_VECTOR2D>& Get2DPoints()const
        {
            return points;
        }

        IFTModel* GetFaceModel() const
        {
            return pFaceTrackerModel;
        }

        const RECT& GetFaceRect()
        {
            return faceRect_;
        }

        bool IsSucceess() const;

    private:

        FaceTrackFrame( FaceTracker& faceTracker );

    private:

        FaceTracker& faceTracker_;

        IFTModel* pFaceTrackerModel;

        RECT faceRect_;

        std::vector<FT_VECTOR2D> points;
    };

}}}
