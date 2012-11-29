#include "KinectSensor.h"
#include "ColorImageStream.h"
#include "ColorImageFrame.h"

namespace NaturalSoftware { namespace Kinect {

    ColorImageFrame::ColorImageFrame( KinectSensor& kinect, ColorImageStream& imageStream, DWORD timeout )
	    : ImageFrame( kinect, imageStream, timeout )
    {
    }

    ColorImageFrame::~ColorImageFrame()
    {
    }

    DWORD ColorImageFrame::GetPixelDataLength() const
    {
        return rect_.size;
    }

    const byte* ColorImageFrame::GetPixelData() const
    {
        return rect_.pBits;
    }

    void ColorImageFrame::CopyPixelDataTo( byte* pixelData ) const
    {
        memcpy( pixelData, GetPixelData(), GetPixelDataLength() );
    }
}}
