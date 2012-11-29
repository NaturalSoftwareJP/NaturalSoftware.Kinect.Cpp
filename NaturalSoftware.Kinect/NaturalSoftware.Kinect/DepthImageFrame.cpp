#include "KinectSensor.h"
#include "DepthImageStream.h"
#include "DepthImageFrame.h"

namespace NaturalSoftware { namespace Kinect {

    DepthImageFrame::DepthImageFrame( KinectSensor& kinect, DepthImageStream& imageStream, DWORD timeout )
	    : ImageFrame( kinect, imageStream, timeout )
    {
        // �t���[���f�[�^�����ɁA�g�������f�[�^���擾����
        BOOL nearMode = FALSE;
        INuiFrameTexture *frameTexture = 0;
        kinect->NuiImageFrameGetDepthImagePixelFrameTexture(
            imageStream.GetHandle(), &frame_, &nearMode, &depthPixelFrameTexture );

        // NUI_DEPTH_IMAGE_PIXEL�����o��
        depthPixelFrameTexture->LockRect( 0, &depthPixelRect, 0, 0 );
    }

    DepthImageFrame::~DepthImageFrame()
    {
        depthPixelFrameTexture->UnlockRect( 0 );
    }

    DWORD DepthImageFrame::GetPixelDataLength() const
    {
	    return rect_.size;
    }

    const USHORT* DepthImageFrame::GetPixelData() const
    {
        return (USHORT*)rect_.pBits;
    }

    void DepthImageFrame::CopyPixelDataTo( USHORT* pixelData ) const
    {
        memcpy( pixelData, GetPixelData(), GetPixelDataLength() );
    }

    void DepthImageFrame::CopyDepthImagePixelDataTo( NUI_DEPTH_IMAGE_PIXEL* pixelData ) const
    {
        memcpy( pixelData, GetDepthPixel(), depthPixelRect.size );
    }

    const NUI_DEPTH_IMAGE_PIXEL* DepthImageFrame::GetDepthPixel() const
    {
        return (NUI_DEPTH_IMAGE_PIXEL*)depthPixelRect.pBits;
    }

    DWORD DepthImageFrame::GetDepthPixelSize() const
    {
        return GetWidth() * GetHeight();
    }
}}
