#include "FaceTracker.h"
#include "FaceTrackFrame.h"

namespace NaturalSoftware { namespace Kinect { namespace FaceTracking {
    FaceTrackFrame::FaceTrackFrame( FaceTracker& faceTracker )
        : faceTracker_( faceTracker )
    {
        // 顔の領域を取得する
        faceTracker_.Result()->GetFaceRect( &faceRect_ );

        // 2Dの座標を取得する
        FT_VECTOR2D* pt = 0;
        UINT count = 0;
        HRESULT hr = faceTracker_.Result()->Get2DShapePoints( &pt, &count );
        if( FAILED(hr) ) {
            ::OutputDebugString( L"Get2DShapePoints Failed\n" );
        }

        points = std::vector<FT_VECTOR2D>( &pt[0], &pt[count] );

        faceTracker_.GetTracker()->GetFaceModel( &pFaceTrackerModel );
    }

    bool FaceTrackFrame::IsSucceess() const 
    {
        return faceTracker_.Result()->GetStatus() == S_OK;
    }
}}}
