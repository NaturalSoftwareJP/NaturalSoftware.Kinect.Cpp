#include <Windows.h>
#include <NaturalSoftware.Kinect.PCL\KinectGrabber.h>

#include <pcl/visualization/cloud_viewer.h>

void main()
{
    try {
        // viewer�����
        pcl::visualization::CloudViewer viewer( "Kinect Viewer" );

        // �f�[�^�X�V�̃R�[���o�b�N�֐�
        boost::function<void (const pcl::PointCloud<pcl::PointXYZRGB>::ConstPtr&)> f = 
            [&viewer](const pcl::PointCloud<pcl::PointXYZRGB>::ConstPtr &cloud) {
                if (!viewer.wasStopped()) {
                    viewer.showCloud( cloud );
                }
        };

        // Kinect�̓��͂����
        pcl::Grabber* grabber = new pcl::KinectGrabber();
        grabber->registerCallback( f );
        grabber->start();
        while ( !viewer.wasStopped() ) {
            ::Sleep( 0 );
        }

        grabber->stop();
    }
    catch ( std::exception& ex ) {
        std::cout << ex.what() << std::endl;
    }
}

