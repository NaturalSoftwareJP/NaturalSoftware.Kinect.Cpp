#pragma once 

#include <Windows.h>

#include <mmsystem.h>
#pragma comment( lib, "winmm.lib" )

#include <vector>

// WAVE�f�[�^���X�g���[�~���O�Đ�����
class StreamingWavePlayer
{
public:

	StreamingWavePlayer()
	{
	}

	virtual ~StreamingWavePlayer()
	{
		close();
	}

	// WAVE�o�͂��J��
	void open( const WAVEFORMATEX* format, int audioBufferCount = 100 )
	{
		audioBufferCount_ = audioBufferCount;
		audioBufferNextIndex_ = 0;

		MMRESULT ret = ::waveOutOpen( &waveHandle_, WAVE_MAPPER, format, NULL, NULL, CALLBACK_NULL );
		if ( ret != MMSYSERR_NOERROR ) {
			throw std::runtime_error( "waveOutOpen failed\n" );
		}

		// �����f�[�^�p�̃o�b�t�@�̍쐬�Ə�����
		waveHeaders_.resize( audioBufferCount_ );
		memset( &waveHeaders_[0], 0, sizeof(WAVEHDR) * waveHeaders_.size() );

		for ( size_t i = 0; i < waveHeaders_.size(); ++i ) {
			waveHeaders_[i].lpData = new char[MAX_BUFFER_SIZE];
			waveHeaders_[i].dwUser = i;
			waveHeaders_[i].dwFlags = WHDR_DONE;
		}
	}

	// WAVE�o�͂��J��
	void open( int sampleRate, int bitsPerSample, int channels, int audioBufferCount = 100 )
	{
		WAVEFORMATEX format;
		format.wFormatTag = WAVE_FORMAT_PCM;
		format.nChannels = channels;
		format.nSamplesPerSec = sampleRate;
		format.wBitsPerSample = bitsPerSample;
		format.nBlockAlign = format.wBitsPerSample * format.nChannels / 8;
		format.nAvgBytesPerSec = format.nBlockAlign * format.nSamplesPerSec;

		open( &format, audioBufferCount );
	}

	// WAVE�o�͂����
	void close()
	{
		for ( size_t i = 0; i < waveHeaders_.size(); ++i ) {
			delete[] waveHeaders_[i].lpData;
			waveHeaders_[i].lpData = 0;
		}

		if ( waveHandle_ != 0 ) {
			::waveOutPause( waveHandle_ );
			::waveOutClose( waveHandle_ );
			waveHandle_ = 0;
		}
	}

	// WAVE�f�[�^���o�͂���
	void output( const std::vector< BYTE >& buffer )
	{
		if ( buffer.size() != 0 ) {
			output( &buffer[0], buffer.size() );
		}
	}

	// WAVE�f�[�^���o�͂���
	void output( const void* buffer, DWORD length )
	{
		if ( length == 0 ) {
			return;
		}

		// �o�b�t�@�̎擾
		WAVEHDR* header = &waveHeaders_[audioBufferNextIndex_];
		if ( (header->dwFlags & WHDR_DONE) == 0 ) {
			return;
		}

		// WAVE�w�b�_�̃N���[���A�b�v
		MMRESULT ret = ::waveOutUnprepareHeader( waveHandle_, header, sizeof(WAVEHDR) );
		if ( ret != MMSYSERR_NOERROR ) {
			return;
		}

		// WAVE�f�[�^�̎擾
		header->dwBufferLength = length;
		header->dwFlags = 0;
		memcpy( header->lpData, buffer, header->dwBufferLength );

		// WAVE�w�b�_�̏�����
		ret = ::waveOutPrepareHeader( waveHandle_, header, sizeof(WAVEHDR) );
		if ( ret != MMSYSERR_NOERROR ) {
			return;
		}

		// WAVE�f�[�^���o�̓L���[�ɓ����
		ret = ::waveOutWrite( waveHandle_, header, sizeof(WAVEHDR) );
		if ( ret != MMSYSERR_NOERROR ) {
			return;
		}

		// ���̃o�b�t�@�C���f�b�N�X
		audioBufferNextIndex_ = (audioBufferNextIndex_ + 1) % audioBufferCount_;
	}

private:

	// �R�s�[���֎~����
	StreamingWavePlayer( const StreamingWavePlayer& rhs );
	StreamingWavePlayer& operator = ( const StreamingWavePlayer& rhs );

private:

	static const int MAX_BUFFER_SIZE = 2 * 1024 * 1024;

	HWAVEOUT							waveHandle_;
	std::vector<WAVEHDR>  waveHeaders_;

	int audioBufferCount_;
	int audioBufferNextIndex_;
};
