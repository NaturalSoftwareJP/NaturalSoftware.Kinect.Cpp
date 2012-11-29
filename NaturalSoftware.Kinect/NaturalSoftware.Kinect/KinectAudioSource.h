#pragma once

#include <Windows.h>

#include <vector>

#include <mfapi.h>          // IPropertyStore
#include <wmcodecdsp.h>     // MFPKEY_WMAAECMA_SYSTEM_MODE
#include <uuids.h>          // DShowRecordの中

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

        // 初期化する
        void Initialize();

        // 終了する
        void Close();

        // マイクアレイ、エコーのキャンセル、ノイズの抑制について設定する
        //   SINGLE_CHANNEL_AEC = 0
        //   OPTIBEAM_ARRAY_ONLY = 2
        //   OPTIBEAM_ARRAY_AND_AEC = 4
        //   SINGLE_CHANNEL_NSAGC = 5
        void SetSystemMode( LONG mode );

        // 音声入力を開始する
        void Start();

        // 音声データを取得する
        std::vector< BYTE > Read();

        // WAVEフォーマットを取得する(固定)
        const WAVEFORMATEX& GetWaveFormat() const
        {
            static const WAVEFORMATEX format = {WAVE_FORMAT_PCM, 1, 16000, 32000, 2, 16, 0};
            return format;
        }

        // 音源方向の信頼性を取得する
        double GetSoundSourceAngleConfidence() const
        {
            return soundSourceAngleConfidence_;
        }

        // 音源方向の値を取得する
        double GetSoundSourceAngle() const
        {
            return soundSourceAngle_;
        }

        // 音源方向推定のための、ビームの方向を取得する
        double GetBeamAngle() const
        {
            return beamAngle_;
        }

    private:

        // 単体でのインスタンスの生成を禁止する
        KinectAudioSource( KinectSensor& sensor );

        // コピーを禁止する
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
