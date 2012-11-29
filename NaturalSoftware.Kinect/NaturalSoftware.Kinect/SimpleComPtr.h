#pragma once

namespace NaturalSoftware { namespace Kinect {

    template<typename T>
    class SimpleComPtr
    {
    public:

        typedef T* Ptr;

        SimpleComPtr()
            : ptr_( 0 )
        {
        }

        ~SimpleComPtr()
        {
            release();
        }

        void set( Ptr ptr )
        {
            ptr_ = ptr;
        }

        void release()
        {
            if ( ptr_ != 0 ) {
                ptr_->Release();
                ptr_ = 0;
            }
        }

        Ptr get()
        {
            return ptr_;
        }

        const Ptr get() const
        {
            return ptr_;
        }

        Ptr* operator & ()
        {
            return &ptr_;
        }

        Ptr operator -> ()
        {
            return ptr_;
        }

        bool operator ! ()
        {
            return ptr_ == 0;
        }

    private:

        Ptr ptr_;
    };
}}
