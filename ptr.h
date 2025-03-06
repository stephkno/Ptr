#ifndef PTR_H
#define PTR_H

    #include <iostream>
    #include <typeinfo>
    #include <string>

    // demangle
    #include <cxxabi.h>
    #include <stdint.h>

    using namespace std;

    class UninitializedPointer{
    public:
        string Msg() {
            return "Tried to access uninitialized pointer object.";
        }
    };

    template <typename T>
    class Ptr {

    public:

        void * operator new (size_t);

        // Constructors
        // create new Smart Ptr of this type
        // default nullptr
        // set init = true to initialize pointer
        Ptr(bool init=false) : ptr_(init?new T():nullptr)
        {
            if(ptr_){
                ref_count_ = new uint32_t(1);
            }else{
                ref_count_ = new uint32_t(0);
            }
        }

        // create a new smart pointer from T by copying T
        Ptr(T & t)
        {
            ref_count_ = new uint32_t(1);
            ptr_ = new T(t);
        }

        // copy constructor
        Ptr(const Ptr<T> & other) : ptr_(other.ptr_), ref_count_(other.ref_count_)
        {

            if(ref_count_){
                ++(*ref_count_);
            }

        }

        Ptr(Ptr<T>&& other) noexcept : ptr_(other.ptr_), ref_count_(other.ref_count_) {
            other.ptr_ = nullptr;
            other.ref_count_ = nullptr;
        }

        // Destructor
        ~Ptr()
        {
            if (ref_count_ != nullptr && ref_count_ == 0)
            {
                delete ptr_;
                delete ref_count_;
            }else if (ref_count_ != nullptr) {
                --(*ref_count_);
            }
        }

        // Assignment operators
        Ptr<T>& operator=(const Ptr<T>& other)
        {
            if (this != &other) {
                // Decrement the reference count of the current object
                if (ref_count_ && --(*ref_count_) == 0) {
                    delete ptr_;
                    delete ref_count_;
                }

                // Assign the new pointer and reference count
                ptr_ = other.ptr_;
                ref_count_ = other.ref_count_;

                // Increment the reference count of the new object
                if (ref_count_) {
                    ++(*ref_count_);
                }
            }
            return *this;
        }

        // Dereferencing operators
        T & operator*() const {
            if(!ptr_){
                throw UninitializedPointer();
            }
            return *ptr_;
        }

        T * operator->() const {
            if(!ptr_){
                throw UninitializedPointer();
            }
            return ptr_;
        }

        T Get() const {
            if(!ptr_){
                throw UninitializedPointer();
            }
            return *ptr_;
        }

        // Utility functions
        bool Unique() const {
            return ref_count_ && *ref_count_ == 1;
        }

        uint32_t Count() const {
            return *ref_count_;
        }

        void Swap(Ptr<T>& other) {
            std::swap(ptr_, other.ptr_);
            std::swap(ref_count_, other.ref_count_);
        }

/*
        // Copy assignment operator.
        Ptr& operator=(const Ptr& other) {
            if (ptr_ != other.ptr_) {
                Ptr tmp(other);
                Swap(tmp);
            }
            return *this;
        }
*/

        // -------------
        // Cast the stored pointer to type T and return it
        template <typename U>
        U * Value() const {
            return dynamic_cast<U*>(ptr_);
        }

        /*
        template <typename U>
        Ptr<U> Cast() const {
            
            U* casted_ptr = dynamic_cast<U*>(ptr_);

            if (casted_ptr == nullptr) {
                // The cast failed, so return an empty Ptr<U> object.
                return Ptr<U>();
            }

            // The cast succeeded, so create a new Ptr<U> object with the same
            // pointer value as the original Ptr<Base> object.
            Ptr<U> result(casted_ptr);
            return result;
        }
        */

        template <typename U>
        Ptr<U> Cast() const {
            return Ptr<U>(dynamic_cast<U*>(ptr_), ref_count_);
        }

        template<typename U>
        bool Same() const {
            return dynamic_cast<const U*>(ptr_) != nullptr;
        }

        operator bool() const {
            return ptr_ != nullptr;
        }

        bool operator==(const Ptr<T>& lhs) {
            return lhs.ptr_ == ptr_;
        }

        string Type(){
            const std::type_info& t = typeid(*ptr_);
            int status;
            char* demangled = abi::__cxa_demangle(t.name(), 0, 0, &status);
            string r(demangled);
            free(demangled);
            return r;
        }

        static Ptr<T> Null;

private:

        T* ptr_;
        uint32_t * ref_count_;

        // Private constructor for dynamic_cast_to
        Ptr(T* ptr, uint32_t* ref_count) : ptr_(ptr), ref_count_(ref_count) {
            
            if (ptr_ != nullptr) {
                ++(*ref_count_);
            }

        }

        template <typename U>
        friend class Ptr;

    };

    template<typename T>
    Ptr<T> Ptr<T>::Null = Ptr<T>();

#endif