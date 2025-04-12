#ifndef PTR_H
#define PTR_H

#include <typeinfo>
#include <string>
#include <iostream>
#include <cxxabi.h>
#include <memory>
#include <sstream>

#include <boost/serialization/serialization.hpp> 
#include <boost/serialization/nvp.hpp> 
#include <boost/serialization/split_member.hpp>

using namespace std;

class UninitializedPointer : public exception {
public:
    const char* what() const noexcept override {
        return "Tried to access uninitialized pointer object.";
    }
};

class PointerAlreadyInitialized : public exception {
public:
    const char* what() const noexcept override {
        return "Tried to reinitialize a pointer that already exists.";
    }
};

template <typename T>
class Ptr {

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
        ar & boost::serialization::make_nvp("pointer", ptr_); // Serialize raw pointer
        //ar & boost::serialization::make_nvp("ref_count", *ref_count_);
    }

private:

    T* ptr_ = nullptr;
    uint32_t* ref_count_ = nullptr;

    // Private constructor for cross-type initialization
    template <typename U>
    Ptr(U* ptr, uint32_t* ref_count) : ptr_(ptr), ref_count_(ref_count) {
        if (ref_count_) ++(*ref_count_);
    }

    explicit Ptr(T* ptr) : ptr_(ptr), ref_count_(new uint32_t(1)) {}

public:
    // Friend all other Ptr templates
    template <typename U>
    friend class Ptr;

    // Prohibit heap allocation
    //void* operator new(size_t) = delete;
    //void operator delete(void*) = delete;

    // Ptr factory method
    template <typename... Args>
    static Ptr<T> Make(Args&&... args) {
        return Ptr<T>(new T(std::forward<Args>(args)...));
    }
    
/*
    explicit Ptr(T* value = nullptr) : ptr_(value), ref_count_(new uint32_t(1)) {
        if (!value) ref_count_ = nullptr;
    }
*/
    Ptr() : ptr_(nullptr), ref_count_(nullptr) {}

    Ptr(std::nullptr_t) : ptr_(nullptr), ref_count_(nullptr) {}
    
    // Copy constructor
    Ptr(const Ptr& other) : ptr_(other.ptr_), ref_count_(other.ref_count_) {
        //free();
        if (ref_count_) ++(*ref_count_);
    }

    // Move constructor
    Ptr(Ptr&& other) noexcept : ptr_(other.ptr_), ref_count_(other.ref_count_) {
        other.ptr_ = nullptr;
        other.ref_count_ = nullptr;
    }
    
    explicit Ptr(const T& value) : ptr_(new T(value)), ref_count_(new uint32_t(1)) {}

    // Destructor
    ~Ptr() {
        free();
    }

    void free() {

        if (ref_count_ && --(*ref_count_) == 0) {

            if(ptr_)
                delete ptr_;
            if(ref_count_)
                delete ref_count_;
        
            ptr_ = nullptr;
            ref_count_ = nullptr;
            
        }

    }

    Ptr& operator=(std::nullptr_t) {
        free();
        return *this;
    }

    // Assignment operator
    Ptr& operator=(const Ptr& other) {
        if (this != &other) {
            free();  // Decrement existing ref_count (if any)
            ptr_ = other.ptr_;
            ref_count_ = other.ref_count_;
            if (ref_count_) {
                ++(*ref_count_);
            }
        }
        return *this;
    }

    // Move assignment operator
    Ptr& operator =(Ptr&& other) noexcept {
        if (this != &other) {
            free();
            ptr_ = other.ptr_;
            ref_count_ = other.ref_count_;
            other.ptr_ = nullptr;
            other.ref_count_ = nullptr;
        }
        return *this;
    }

    bool operator <(const T * other){
        return ptr_ < other;
    }
    bool operator >(const T * other){
        return ptr_ > other;
    }
    bool operator ==(const Ptr & other){
        return ptr_ == other.ptr_;
    }
    bool operator !=(const Ptr & other){
        return ptr_ != other.ptr_;
    }
    bool operator ==(const T * other){
        return ptr_ == other;
    }
    bool operator !=(const T * other){
        return ptr_ != other;
    }
    
    // Check if initialized
    explicit operator bool() const noexcept {
        return ptr_ != nullptr;
    }

    friend ostream& operator<<(ostream& os, const Ptr& ptr){
        os << ptr.ptr_;
        return os;
    }

    // Dereference operators
    T& operator*() const {
        if (!ptr_) throw UninitializedPointer();
        return *ptr_;
    }

    T* operator->() const {
        if (!ptr_) throw UninitializedPointer();
        return ptr_;
    }
  
    string Addr(){
        const void * address = static_cast<const void*>(ptr_);
        stringstream ss;
        ss << address;
        return ss.str();
    }

    T & Get() const noexcept {
        return *ptr_;
    }

    // Get reference count
    uint32_t Count() const noexcept {
        return ref_count_ ? *ref_count_ : 0;
    }

    // Check uniqueness
    bool Unique() const noexcept {
        return ref_count_ && *ref_count_ == 1;
    }

    // Type information
    string Type() const {
        if (!ptr_) return "Null";
        int status;
        char* demangled = abi::__cxa_demangle(typeid(*ptr_).name(), 0, 0, &status);
        string result(demangled);
        return result;
    }

    // Safe casting method
    template <typename U>
    Ptr<U> Cast() const {
        U* casted = dynamic_cast<U*>(ptr_);
        if (!casted) return Ptr<U>();  // Return null if cast failed
    
        // Create a new Ptr<U> instance with a separate reference count
        // This assumes the casted object should share the same ownership semantics as the original.
        Ptr<U> newPtr(casted, ref_count_);
        return newPtr;
    }

};

#endif
