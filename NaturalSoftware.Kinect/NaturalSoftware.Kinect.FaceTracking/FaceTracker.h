// Thaks : http://www.codeproject.com/Articles/394975/How-To-Use-Kinect-Face-Tracking-SDK
#pragma once 

#include <vector>

#include "../NaturalSoftware.Kinect/KinectSensor.h"
#include "../NaturalSoftware.Kinect/SimpleComPtr.h"
#include <FaceTrackLib.h>

#include "FaceTrackFrame.h"

namespace ns = NaturalSoftware::Kinect;

namespace NaturalSoftware { namespace Kinect { namespace FaceTracking {

    class FaceTracker
    {
    public:

        FaceTracker( KinectSensor& kinectSensor );
        ~FaceTracker();

        FaceTrackFrame Track( const ColorImageFrame& colorFrame, const DepthImageFrame& depthFrame );
        FaceTrackFrame Track( const ColorImageFrame& colorFrame, const DepthImageFrame& depthFrame, const NUI_SKELETON_DATA* skeleton );

        bool IsFaceTracked() const;

        IFTFaceTracker* GetTracker() const
        {
            return pFaceTracker.get();
        }

        IFTResult* Result() const
        {
            return pFaceTrackerResult.get();
        }

    private:

        void InitializeFaceTracker();

        FT_VECTOR3D JointToVector3D( const NUI_SKELETON_DATA* skeleton, NUI_SKELETON_POSITION_INDEX jointType );

        // ��ǐ�(�����p)
        HRESULT FaceTracking( const ColorImageFrame& colorFrame, const DepthImageFrame& depthFrame, const NUI_SKELETON_DATA* skeleton );

    private:

        KinectSensor& kinectSensor_;

        SimpleComPtr<IFTFaceTracker> pFaceTracker;
        SimpleComPtr<IFTResult> pFaceTrackerResult;        // ��ǐՌ���
        SimpleComPtr<IFTImage> pColorFrame;                // ��ǐ՗p��RGB�f�[�^
        SimpleComPtr<IFTImage> pDepthFrame;                // ��ǐ՗p�̋����f�[�^

        FT_CAMERA_CONFIG videoCameraConfig; // RGB�J�����̐ݒ�
        FT_CAMERA_CONFIG depthCameraConfig; // �����J�����̐ݒ�
        FT_SENSOR_DATA sensorData;          // Kinect�Z���T�[�f�[�^

        std::vector<unsigned char> colorCameraFrameBuffer;
        std::vector<unsigned char> depthCameraFrameBuffer;

        bool isFaceTracked_;
    };
}}}
