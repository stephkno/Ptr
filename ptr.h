#ifndef PTR_H
#define PTR_H
#define P Ptr

#include <boost/serialization/serialization.hpp> 
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/binary_object.hpp>

#include <iostream>
#include <typeinfo>
#include <cxxabi.h>

template <typename T>
class Ptr {
    
    friend class boost::serialization::access;
        
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        if(Archive::is_loading::value){
            ptr_ = new T();
        }
        ar & ptr_;
        ar & *ref_count_;
    }

public:

    // Constructors
    // create new Smart Ptr of this type
    // default nullptr
    // set init = true to initialize pointer
    Ptr(bool init=false, bool verbose=false) : ptr_(init?new T():nullptr), verbose(verbose), ref_count_(new uint(0))
    {
#ifdef LOG
        Log << "Create new pointer " << ptr_;
#endif
    }
    
    // create a NEW smart pointer from this pointer
    Ptr(T * t, bool verbose=false) : verbose(verbose), ref_count_(new uint(0))
    {
        
        ptr_ = t;
#ifdef LOG
            Log << "Create pointer from: " << t << " new: " << ptr_;
#endif
    
    }

    // copy constructor
    Ptr(const Ptr<T>& other) : ptr_(other.ptr_)
    {
        ref_count_ = other.ref_count_;
#ifdef LOG
            Log << "Copy pointer: " << &ptr_;
#endif
        if(ptr_!=nullptr){
            ++(*ref_count_);
        }
#ifdef LOG
            Log << ref_count_;
#endif

    }

    // Destructor
    ~Ptr()
    {
        if (ref_count_ && ref_count_-1 == 0)
        {
            ref_count_=0;
            delete ptr_;
        }else{
            --(*ref_count_);
        }
    }

#ifdef false
    // Assignment operators
    Ptr<T>& operator=(const Ptr<T>& other)
    {
        if (*this != other) {
            // Decrement the reference count of the current object
            if (ref_count_ && --ref_count_ == 0) {
                delete ptr_;
            }

            // Assign the new pointer and reference count
            ptr_ = other.ptr_;
            ref_count_ = other.ref_count_;

            // Increment the reference count of the new object
            if (ref_count_) {
                ++ref_count_;
            }
        }
        return *this;
    }

    Ptr<T>& operator=(T* ptr)
    {
        // Decrement the reference count of the current object
        if (ref_count_ && --ref_count_ == 0) {
            delete ptr_;
        }

        // Assign the new pointer and reference count
        ptr_ = ptr;
        try {
            ref_count_ = 1;
        } catch (...) {
            delete ptr_;
            throw;
        }

        return *this;
    }
#endif
 
    // Dereferencing operators
    T& operator*() const {
        return *ptr_;
    }

    T* operator->() const {
        return ptr_;
    }

    T* operator&() const {
        return ptr_;
    }

    // Utility functions
    bool unique() const {
        return ref_count_ && *ref_count_ == 1;
    }

    uint count() const {
        return *ref_count_;
    }

    T* get() const {
        return ptr_;
    }

    // -------------
    void reset() {
        Ptr<T>().swap(*this);
    }

    void reset(T* ptr) {
        Ptr<T>(ptr).swap(*this);
    }

    void swap(Ptr<T>& other) {
        std::swap(ptr_, other.ptr_);
        std::swap(ref_count_, other.ref_count_);
    }

    // Copy assignment operator.
    Ptr& operator=(const Ptr& other) {
        if (ptr_ != &other) {
            Ptr tmp(other);
            swap(tmp);
        }
        return *this;
    }
    
    // -------------
    // Cast the stored pointer to type and return it
    template <typename U>
    U * Value() const {
        return dynamic_cast<U*>(ptr_);
    }
        
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

    const char * Name(){
        const std::type_info& t = typeid(*ptr_);
        int status;
        char* demangled = abi::__cxa_demangle(t.name(), 0, 0, &status);
        return demangled;
    }

    static Ptr<T> Null;

private:
    T* ptr_;
    uint * ref_count_;
    bool verbose = false;
};

template<typename T>
Ptr<T> Ptr<T>::Null = Ptr<T>();

#endif