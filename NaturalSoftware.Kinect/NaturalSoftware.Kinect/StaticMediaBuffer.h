#pragma once 

#include <Windows.h>
#include <dmo.h> // IMediaObject
#include <vector>

namespace NaturalSoftware { namespace Kinect {

    // ���f�[�^���擾���邽�߂̃o�b�t�@�BKinect SDK�̃T���v��������p����ђǉ�
    class CStaticMediaBuffer : public IMediaBuffer {
    public:

        CStaticMediaBuffer()
        {
        }

        STDMETHODIMP_(ULONG) AddRef()
        {
            return 2;
        }

        STDMETHODIMP_(ULONG) Release()
        {
            return 1;
        }

        STDMETHODIMP QueryInterface( REFIID riid, void **ppv )
        {
            if (riid == IID_IUnknown) {
                AddRef();
                *ppv = (IUnknown*)this;
                return NOERROR;
            }
            else if (riid == IID_IMediaBuffer) {
                AddRef();
                *ppv = (IMediaBuffer*)this;
                return NOERROR;
            }
            else {
                return E_NOINTERFACE;
            }
        }

        // ���݂̃f�[�^����ݒ肷��(IMediaBuffer�̎���)
        STDMETHODIMP SetLength( DWORD ulLength ) {
            m_ulData = ulLength;
            return NOERROR;
        }

        // �o�b�t�@�̍ő咷���擾����(IMediaBuffer�̎���)
        STDMETHODIMP GetMaxLength(DWORD *pcbMaxLength) {
            *pcbMaxLength = buffer_.size();
            return NOERROR;
        }

        // �o�b�t�@����ь��݂̃f�[�^�����擾����(IMediaBuffer�̎���)
        STDMETHODIMP GetBufferAndLength(BYTE **ppBuffer, DWORD *pcbLength) {
            if ( ppBuffer ) {
                *ppBuffer = &buffer_[0];
            }

            if ( pcbLength ) {
                *pcbLength = m_ulData;
            }

            return NOERROR;
        }

        // �f�[�^�����N���A����
        void Clear()
        {
            m_ulData = 0;
        }

        // �f�[�^�����擾����
        ULONG GetDataLength() const
        {
            return m_ulData;
        }

        // �o�b�t�@�T�C�Y��ݒ肷��
        void SetBufferLength( ULONG length )
        {
            buffer_.resize( length );
        }

        // �o�b�t�@���R�s�[����
        std::vector< BYTE > Clone() const
        {
            return std::vector< BYTE >( buffer_.begin(), buffer_.begin() + GetDataLength() );
        }

    protected:

        std::vector< BYTE > buffer_;	// �o�b�t�@
        ULONG               m_ulData;	// �f�[�^��
    };
}}
