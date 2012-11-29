#include "stdafx.h"
#include "CppUnitTest.h"

#include "../NaturalSoftware.Kinect\KinectSensor.h"

#include <sstream>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft{ namespace VisualStudio {namespace CppUnitTestFramework
{
    template <> 
    static std::wstring ToString(const NUI_IMAGE_TYPE& type)
    {
        std::wstringstream ss;
        ss << type << std::endl;
        return ss.str();
    }

    template <> 
    static std::wstring ToString(const NUI_IMAGE_RESOLUTION& type)
    {
        std::wstringstream ss;
        ss << type << std::endl;
        return ss.str();
    }
}}}

namespace NaturalSoftwareKinectTest
{
	TEST_CLASS(UtilityTest)
	{
	public:
		
		TEST_METHOD(ColorImageFormatToImageTypeのテスト)
		{
            Assert::AreEqual( NUI_IMAGE_TYPE_COLOR, ns::ColorImageStream::ColorImageFormatToImageType( ns::ColorImageFormat::RgbResolution640x480Fps30 ) );
            Assert::AreEqual( NUI_IMAGE_TYPE_COLOR, ns::ColorImageStream::ColorImageFormatToImageType( ns::ColorImageFormat::RgbResolution1280x960Fps12 ) );

            Assert::AreEqual( NUI_IMAGE_TYPE_COLOR_RAW_YUV, ns::ColorImageStream::ColorImageFormatToImageType( ns::ColorImageFormat::RawYuvResolution640x480Fps15 ) );

            Assert::AreEqual( NUI_IMAGE_TYPE_COLOR_YUV, ns::ColorImageStream::ColorImageFormatToImageType( ns::ColorImageFormat::YuvResolution640x480Fps15 ) );

            Assert::AreEqual( NUI_IMAGE_TYPE_COLOR_INFRARED, ns::ColorImageStream::ColorImageFormatToImageType( ns::ColorImageFormat::InfraredResolution640x480Fps30 ) );

            Assert::AreEqual( NUI_IMAGE_TYPE_COLOR_RAW_BAYER, ns::ColorImageStream::ColorImageFormatToImageType( ns::ColorImageFormat::RawBayerResolution1280x960Fps12 ) );
            Assert::AreEqual( NUI_IMAGE_TYPE_COLOR_RAW_BAYER, ns::ColorImageStream::ColorImageFormatToImageType( ns::ColorImageFormat::RawBayerResolution640x480Fps30 ) );
		}
		
		TEST_METHOD(ColorImageFormatToResolutionのテスト)
		{
            Assert::AreEqual( NUI_IMAGE_RESOLUTION_640x480, ns::ColorImageStream::ColorImageFormatToResolution( ns::ColorImageFormat::RgbResolution640x480Fps30 ) );
            Assert::AreEqual( NUI_IMAGE_RESOLUTION_640x480, ns::ColorImageStream::ColorImageFormatToResolution( ns::ColorImageFormat::RawYuvResolution640x480Fps15 ) );
            Assert::AreEqual( NUI_IMAGE_RESOLUTION_640x480, ns::ColorImageStream::ColorImageFormatToResolution( ns::ColorImageFormat::YuvResolution640x480Fps15 ) );
            Assert::AreEqual( NUI_IMAGE_RESOLUTION_640x480, ns::ColorImageStream::ColorImageFormatToResolution( ns::ColorImageFormat::InfraredResolution640x480Fps30 ) );
            Assert::AreEqual( NUI_IMAGE_RESOLUTION_640x480, ns::ColorImageStream::ColorImageFormatToResolution( ns::ColorImageFormat::RawBayerResolution640x480Fps30 ) );

            Assert::AreEqual( NUI_IMAGE_RESOLUTION_1280x960, ns::ColorImageStream::ColorImageFormatToResolution( ns::ColorImageFormat::RgbResolution1280x960Fps12 ) );
            Assert::AreEqual( NUI_IMAGE_RESOLUTION_1280x960, ns::ColorImageStream::ColorImageFormatToResolution( ns::ColorImageFormat::RawBayerResolution1280x960Fps12 ) );
        }
		
		TEST_METHOD(DepthImageFormatToResolutionのテスト)
		{
            Assert::AreEqual( NUI_IMAGE_RESOLUTION_80x60, ns::DepthImageStream::DepthImageFormatToResolution( ns::DepthImageFormat::Resolution80x60Fps30 ) );
            Assert::AreEqual( NUI_IMAGE_RESOLUTION_320x240, ns::DepthImageStream::DepthImageFormatToResolution( ns::DepthImageFormat::Resolution320x240Fps30 ) );
            Assert::AreEqual( NUI_IMAGE_RESOLUTION_640x480, ns::DepthImageStream::DepthImageFormatToResolution( ns::DepthImageFormat::Resolution640x480Fps30 ) );
        }
	};
}
