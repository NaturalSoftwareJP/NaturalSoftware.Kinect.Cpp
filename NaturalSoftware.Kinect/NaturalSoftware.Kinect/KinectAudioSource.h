#pragma once

#include <Windows.h>

#include <vector>

#include <mfapi.h>          // IPropertyStore
#include <wmcodecdsp.h>     // MFPKEY_WMAAECMA_SYSTEM_MODE
#include <uuids.h>          // DShowRecord�̒�

#include <NuiApi.h>

#include "SimpleComPtr.h"
#include "StaticMediaBuffer.h"

#pragma comment( lib, "Msdmo.lib" )
#pragma comment( lib, "dmoguids.lib" ) // IMediaObject
#pragma comment( lib, "amstrmid.lib" )

namespace NaturalSoftware { namespace Kinect {

    class KinectSensor;

    class KinectAudioSource
    {
        friend class KinectSensor;

    public:

        ~KinectAudioSource();

        // ����������
        void Initialize();

        // �I������
        void Close();

        // �}�C�N�A���C�A�G�R�[�̃L�����Z���A�m�C�Y�̗}���ɂ��Đݒ肷��
        //   SINGLE_CHANNEL_AEC = 0
        //   OPTIBEAM_ARRAY_ONLY = 2
        //   OPTIBEAM_ARRAY_AND_AEC = 4
        //   SINGLE_CHANNEL_NSAGC = 5
        void SetSystemMode( LONG mode );

        // �������͂��J�n����
        void Start();

        // �����f�[�^���擾����
        std::vector< BYTE > Read();

        // WAVE�t�H�[�}�b�g���擾����(�Œ�)
        const WAVEFORMATEX& GetWaveFormat() const
        {
            static const WAVEFORMATEX format = {WAVE_FORMAT_PCM, 1, 16000, 32000, 2, 16, 0};
            return format;
        }

        // ���������̐M�������擾����
        double GetSoundSourceAngleConfidence() const
        {
            return soundSourceAngleConfidence_;
        }

        // ���������̒l���擾����
        double GetSoundSourceAngle() const
        {
            return soundSourceAngle_;
        }

        // ������������̂��߂́A�r�[���̕������擾����
        double GetBeamAngle() const
        {
            return beamAngle_;
        }

    private:

        // �P�̂ł̃C���X�^���X�̐������֎~����
        KinectAudioSource( KinectSensor& sensor );

        // �R�s�[���֎~����
        KinectAudioSource( const KinectAudioSource& rhs );
        KinectAudioSource& operator = ( const KinectAudioSource& rhs );

    private:

        KinectSensor& kinectSensor_;
        SimpleComPtr<INuiAudioBeam>     soundSource_;
        SimpleComPtr<IMediaObject>      mediaObject_;  
        SimpleComPtr<IPropertyStore>    propertyStore_;

        CStaticMediaBuffer mediaBuffer_;
        DMO_OUTPUT_DATA_BUFFER outputBufferStruct_;

        double beamAngle_;
        double soundSourceAngle_;
        double soundSourceAngleConfidence_;	

    };
}}
