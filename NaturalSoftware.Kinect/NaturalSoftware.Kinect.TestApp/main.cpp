#include <NaturalSoftware.Kinect\KinectSensor.h>
#include <NaturalSoftware.Kinect.FaceTracking\FaceTracker.h>

#include <opencv2\opencv.hpp>

#include "StreamingWavePlayer.h"

void drawJoint( ns::KinectSensor& kinect, cv::Mat& image, const ns::Joint& joint )
{
    if ( joint.TrackingState == NUI_SKELETON_POSITION_NOT_TRACKED ) {
        return;
    }

    INuiCoordinateMapper *pMapping;
    kinect->NuiGetCoordinateMapper(&pMapping);

    NUI_COLOR_IMAGE_POINT pt;
    pMapping->MapSkeletonPointToColorPoint( (Vector4*)&joint.Position,
        kinect.ColorImageStream().GetImageType(),
        kinect.ColorImageStream().GetResolution(),
        &pt );

    cv::circle( image, cv::Point( pt.x, pt.y ), 10, cv::Scalar( 0, 255, 0 ), 5 );
}

void main()
{
    try {
        ns::KinectSensor& kinect = ns::KinectSensor::KinectSensors().Find( 
            []( ns::KinectSensor& k ){ return k.Status() == S_OK; } );

        kinect.ColorImageStream().Enable( ns::ColorImageFormat::RgbResolution640x480Fps30 );

        kinect.DepthImageStream().Enable( ns::DepthImageFormat::Resolution640x480Fps30 );
        kinect.DepthImageStream().SetRange( ns::DepthRange::Near );

        kinect.SkeletonStream().Enable();
        kinect.SkeletonStream().SetTrackingMode( ns::SkeletonTrackingMode::Seated );
        kinect.SkeletonStream().EnableTrackingInNearRange( true );

        kinect.Start();

        kinect.AudioSource().Start();
        kinect.AudioSource().SetSystemMode( 4 );


        StreamingWavePlayer player;
        player.open( &kinect.AudioSource().GetWaveFormat() );

        ns::FaceTracking::FaceTracker faceTracker( kinect );

        while ( 1 ) {
            ns::ColorImageFrame colorFrame = kinect.ColorImageStream().OpenNextFrame();

            int type = CV_8UC4;
            if ( colorFrame.GetBytesPerPixel() == 1 ) {
                type = CV_8U;
            }
            else if ( colorFrame.GetBytesPerPixel() == 2 ) {
                type = CV_16U;
            }

            cv::Mat image( (int)colorFrame.GetHeight(), (int)colorFrame.GetWidth(),
                type, (byte*)colorFrame.GetPixelData() );

            ns::DepthImageFrame depthFrame = kinect.DepthImageStream().OpenNextFrame();
            cv::Mat depthImage( (int)depthFrame.GetHeight(), (int)depthFrame.GetWidth(),
                CV_16UC1, (byte*)depthFrame.GetPixelData() );

            // スケルトンを使用しない顔検出
            //auto faceFrame = faceTracker.track( colorFrame, depthFrame );
            //if ( faceFrame.isSucceess() ) {
            //    for ( auto pt : faceFrame.get2DPoints() ) {
            //        cv::circle( image, cv::Point( pt.x, pt.y ), 1, cv::Scalar( 0, 0, 255 ) );
            //    }
            //}

            ns::SkeletonFrame skeletonFrame = kinect.SkeletonStream().OpenNextFrame();
            for ( auto skeleton : skeletonFrame.GetSkeletonData() ) {
                if ( skeleton.TrackingState() == NUI_SKELETON_TRACKING_STATE::NUI_SKELETON_TRACKED ) {
                    for ( int i = 0; i < NUI_SKELETON_POSITION_COUNT; ++i ) {
                        drawJoint( kinect, image, skeleton.GetJoints()[i] );
                    }

                    // スケルトンを使用した顔検出
                    auto faceFrame = faceTracker.Track( colorFrame, depthFrame, skeleton.GetSkeletonData() );
                    if ( faceFrame.IsSucceess() ) {
                        for ( auto pt : faceFrame.Get2DPoints() ) {
                            cv::circle( image, cv::Point( pt.x, pt.y ), 1, cv::Scalar( 0, 0, 255 ) );
                        }
                    }
                }
            }

            cv::imshow( "ColorCamera", image );
            cv::imshow( "DepthCamera", depthImage );

            int key = cv::waitKey( 10 );
            if ( key == 'q' ) {
                break;
            }
            else if ( key == 'c' ) {
                auto format = kinect.ColorImageStream().GetFormat();
                if ( format == ns::ColorImageFormat::RawBayerResolution1280x960Fps12 ) {
                    format = ns::ColorImageFormat::RgbResolution640x480Fps30;
                }
                else {
                    format = (ns::ColorImageFormat)(format + 1);
                }

                kinect.ColorImageStream().Enable( format );
            }
            else if ( key == 'd' ) {
                auto format = kinect.DepthImageStream().GetFormat();
                if ( format == ns::DepthImageFormat::Resolution80x60Fps30 ) {
                    format = ns::DepthImageFormat::Resolution640x480Fps30;
                }
                else {
                    format = (ns::DepthImageFormat)(format + 1);
                }

                kinect.DepthImageStream().Enable( format );
            }

            player.output( kinect.AudioSource().Read() );
        }
    }
    catch ( std::exception& ex ) {
        std::cout << ex.what() << std::endl;
    }
}
