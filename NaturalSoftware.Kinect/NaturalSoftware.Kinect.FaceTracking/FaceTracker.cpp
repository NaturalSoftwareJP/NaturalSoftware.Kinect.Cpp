#include "FaceTracker.h"

namespace NaturalSoftware { namespace Kinect { namespace FaceTracking {

    FaceTracker::FaceTracker( KinectSensor& kinectSensor )
        : kinectSensor_( kinectSensor )
        , isFaceTracked_( false )
    {
        InitializeFaceTracker();
    }

    FaceTracker::~FaceTracker()
    {
    }

    void FaceTracker::InitializeFaceTracker()
    {
        // FaceTrackerのインスタンスを生成する
        pFaceTracker.set( ::FTCreateFaceTracker() );
        if( !pFaceTracker ) {
            throw std::runtime_error( "ERROR:FTCreateFaceTracker" );
        }

        // RGBカメラおよび距離カメラの設定を行う
        videoCameraConfig.Width = kinectSensor_.ColorImageStream().GetWidth();
        videoCameraConfig.Height = kinectSensor_.ColorImageStream().GetHeight();
        videoCameraConfig.FocalLength =
                    NUI_CAMERA_COLOR_NOMINAL_FOCAL_LENGTH_IN_PIXELS * (videoCameraConfig.Width / 640.0f);

        depthCameraConfig.Width = kinectSensor_.DepthImageStream().GetWidth();
        depthCameraConfig.Height = kinectSensor_.DepthImageStream().GetHeight();
        depthCameraConfig.FocalLength = 
                    NUI_CAMERA_DEPTH_NOMINAL_FOCAL_LENGTH_IN_PIXELS * (depthCameraConfig.Width / 320.0f);

        // FaceTrackerを初期化する
        HRESULT hr = pFaceTracker->Initialize( &videoCameraConfig, &depthCameraConfig, 0, 0) ;
        if( FAILED(hr) ) {
            throw std::runtime_error( "ERROR:Initialize" );
        }

        // FaceTrackerの結果を格納先を生成する
        hr = pFaceTracker->CreateFTResult( &pFaceTrackerResult );
        if(FAILED(hr)) {
            throw std::runtime_error( "ERROR:CreateFTResult" );
        }

        // FaceTrackerで利用するRGBおよび距離データのインスタンスを生成する
        pColorFrame.set( ::FTCreateImage() );
        pDepthFrame.set( ::FTCreateImage() );
        if( !pColorFrame || !pDepthFrame ) {
            throw std::runtime_error( "ERROR:FTCreateImage" );
        }

        // RGBおよび距離データのバッファサイズを設定する
        // RGBは1pixelあたり4バイト。距離は1pixelあたり2バイト
        colorCameraFrameBuffer.resize( videoCameraConfig.Width*4 * videoCameraConfig.Height );
        depthCameraFrameBuffer.resize( depthCameraConfig.Width*2 * depthCameraConfig.Height );

        // フレームデータにバッファを設定する
        // You can also use Allocate() method in which case IFTImage interfaces own their memory.
        // In this case use CopyTo() method to copy buffers
        // CopyToでもOK?
        pColorFrame->Attach( videoCameraConfig.Width, videoCameraConfig.Height,
            &colorCameraFrameBuffer[0], FTIMAGEFORMAT_UINT8_B8G8R8X8, videoCameraConfig.Width * 4 );
        pDepthFrame->Attach( depthCameraConfig.Width, depthCameraConfig.Height,
            &depthCameraFrameBuffer[0], FTIMAGEFORMAT_UINT16_D13P3, depthCameraConfig.Width * 2 );

        // センサーデータを作成する
        sensorData.pVideoFrame = pColorFrame.get();
        sensorData.pDepthFrame = pDepthFrame.get();
        sensorData.ZoomFactor = 1.0f;			// Not used must be 1.0
        sensorData.ViewOffset.x = 0;			// Not used must be (0,0)
        sensorData.ViewOffset.y = 0;			// Not used must be (0,0)
    }

    FaceTrackFrame FaceTracker::Track( const ColorImageFrame& colorFrame, const DepthImageFrame& depthFrame, const NUI_SKELETON_DATA* skeleton )
    {
        // 顔追跡
        HRESULT hr = FaceTracking( colorFrame, depthFrame, skeleton );

        // 顔を見つけたので、追跡状態へ遷移
        if(SUCCEEDED(hr) && SUCCEEDED(pFaceTrackerResult->GetStatus())) {
            isFaceTracked_ = true;
        }
        // 顔を見失ったので、未追跡状態のまま
        else {
            isFaceTracked_ = false;
        }

        return FaceTrackFrame( *this );
    }

    FaceTrackFrame FaceTracker::Track( const ColorImageFrame& colorFrame, const DepthImageFrame& depthFrame )
    {
        return Track( colorFrame, depthFrame, 0 );
    }

    bool FaceTracker::IsFaceTracked() const
    {
        return isFaceTracked_;
    }

    FT_VECTOR3D FaceTracker::JointToVector3D( const NUI_SKELETON_DATA* skeleton, NUI_SKELETON_POSITION_INDEX jointType )
    {
        FT_VECTOR3D v( 0, 0, 0 );
        if ( skeleton->eSkeletonPositionTrackingState[jointType] != NUI_SKELETON_POSITION_TRACKING_STATE::NUI_SKELETON_POSITION_NOT_TRACKED ) {
            auto joint = skeleton->SkeletonPositions[jointType];
            v.x = joint.x;
            v.y = joint.y;
            v.z = joint.z;
        }

        return v;
    }

    // 顔追跡(内部用)
    HRESULT FaceTracker::FaceTracking( const ColorImageFrame& colorFrame, const DepthImageFrame& depthFrame, const NUI_SKELETON_DATA* skeleton )
    {
        // データを更新する
        memcpy( &colorCameraFrameBuffer[0], colorFrame.GetPixelData(), colorFrame.GetPixelDataLength() );
        memcpy( &depthCameraFrameBuffer[0], depthFrame.GetPixelData(), depthFrame.GetPixelDataLength() );

        // スケルトンの座標を設定する(スケルトンが指定されていない場合は0を渡す)
        FT_VECTOR3D* headPointsPtr = 0;
        FT_VECTOR3D headPoints[2] = { FT_VECTOR3D( 0, 0, 0 ), FT_VECTOR3D( 0, 0, 0 ) };
        if ( skeleton != 0 ) {
            if ( skeleton->eTrackingState == NUI_SKELETON_TRACKING_STATE::NUI_SKELETON_TRACKED ) {
                headPoints[0] = JointToVector3D( skeleton, NUI_SKELETON_POSITION_SHOULDER_CENTER );
                headPoints[1] = JointToVector3D( skeleton, NUI_SKELETON_POSITION_HEAD );
                headPointsPtr = headPoints;
            }
        }

        // 追跡中、未追跡によって処理が変わる
        if ( !IsFaceTracked() ) {
            // FaceTrackingを開始する
            return pFaceTracker->StartTracking( &sensorData, 0, headPointsPtr, pFaceTrackerResult.get() );
        }
        else {
            // FaceTrackingを継続する
            return pFaceTracker->ContinueTracking( &sensorData, headPointsPtr, pFaceTrackerResult.get() );
        }
    }

}}}
