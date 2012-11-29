// thnks:http://code.google.com/p/kinect-mssdk-openni-bridge/
#include "KinectAudioSource.h"
#include "KinectSensor.h"

namespace NaturalSoftware { namespace Kinect {

    KinectAudioSource::KinectAudioSource( KinectSensor& kinectSensor )
        : kinectSensor_( kinectSensor )
    {
    
    }


    KinectAudioSource::~KinectAudioSource()
    {
        Close();
    }

    void KinectAudioSource::Initialize()
    {
        Close();

        ERROR_CHECK( kinectSensor_->NuiGetAudioSource( &soundSource_ ) );
        ERROR_CHECK( soundSource_->QueryInterface( &mediaObject_ ) );
        ERROR_CHECK( soundSource_->QueryInterface( &propertyStore_ ) );
    }

    // �I������
    void KinectAudioSource::Close()
    {
        soundSource_.release();
        mediaObject_.release();
        propertyStore_.release();
    }

    // �}�C�N�A���C�A�G�R�[�̃L�����Z���A�m�C�Y�̗}���ɂ��Đݒ肷��
    //   SINGLE_CHANNEL_AEC = 0
    //   OPTIBEAM_ARRAY_ONLY = 2
    //   OPTIBEAM_ARRAY_AND_AEC = 4
    //   SINGLE_CHANNEL_NSAGC = 5
    void KinectAudioSource::SetSystemMode( LONG mode )
    {
        // Set AEC-MicArray DMO system mode.
        // This must be set for the DMO to work properly
        PROPVARIANT prop;
        ::PropVariantInit( &prop );
        prop.vt = VT_I4;
        prop.lVal = mode;
        ERROR_CHECK( propertyStore_->SetValue( MFPKEY_WMAAECMA_SYSTEM_MODE, prop ) );
        ::PropVariantClear( &prop );
    }

    // �������͂��J�n����
    void KinectAudioSource::Start()
    {
        // ����������
        Initialize();

        // �o�b�t�@���Z�b�g����
        mediaBuffer_.SetBufferLength( GetWaveFormat().nSamplesPerSec * GetWaveFormat().nBlockAlign );

        memset( &outputBufferStruct_, 0, sizeof(outputBufferStruct_) );
        outputBufferStruct_.pBuffer = &mediaBuffer_;

        // �������͂̐ݒ���s��
        DMO_MEDIA_TYPE mt = {0};
        ERROR_CHECK( ::MoInitMediaType( &mt, sizeof(WAVEFORMATEX) ) );

        mt.majortype = MEDIATYPE_Audio;
        mt.subtype = MEDIASUBTYPE_PCM;
        mt.lSampleSize = 0;
        mt.bFixedSizeSamples = TRUE;
        mt.bTemporalCompression = FALSE;
        mt.formattype = FORMAT_WaveFormatEx;
        memcpy( mt.pbFormat, &GetWaveFormat(), sizeof(WAVEFORMATEX) );

        ERROR_CHECK( mediaObject_->SetOutputType( 0, &mt, 0 ) );
        ::MoFreeMediaType( &mt );
    }

    // �����f�[�^���擾����
    std::vector< BYTE > KinectAudioSource::Read()
    {
        mediaBuffer_.Clear();

        // �����f�[�^���擾����
        DWORD dwStatus;
        HRESULT ret = mediaObject_->ProcessOutput(0, 1, &outputBufferStruct_, &dwStatus);
        if ( ret == S_OK ) {
            // �܂��f�[�^������
            // outputBufferStruct_.dwStatus & DMO_OUTPUT_DATA_BUFFERF_INCOMPLETE

            // �r�[���Ɖ����̕������擾����
            ERROR_CHECK( soundSource_->GetBeam( &beamAngle_ ) );
            ERROR_CHECK( soundSource_->GetPosition( &soundSourceAngle_, &soundSourceAngleConfidence_ ) );
        }

        return mediaBuffer_.Clone();
    }
}}
