#ifndef PTR_H
#define PTR_H

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

        // Prohibit heap allocation of Ptr
        void * operator new (size_t);

        // Initialize new Ptr of type T
        Ptr(bool init=false) : ptr_(init?new T():nullptr), ref_count_(ptr_?new uint32_t(1):new uint32_t(0))
        {}

        // create a new smart pointer from T by copying T
        Ptr(T t) : ptr_(new T(t)), ref_count_(new uint32_t(1))
        {}

        // copy constructor
        Ptr(const Ptr<T> & other) : ptr_(other.ptr_), ref_count_(other.ref_count_)
        {
            if(ref_count_){
                ++(*ref_count_);
            }
        }
        
        // move constructor
        Ptr(Ptr<T>&& other) noexcept : ptr_(other.ptr_), ref_count_(other.ref_count_) {
            other.ptr_ = nullptr;
            other.ref_count_ = nullptr;
        }

        // Destructor
        ~Ptr()
        {
            // delete pointer if reference count reach 0
            if (ref_count_ != nullptr && ref_count_ == 0)
            {
                delete ptr_;
                delete ref_count_;
            // else decrement reference count
            }else if (ref_count_ != nullptr) {
                --(*ref_count_);
            }
        }

        // Assignment operator for other Ptr<T>
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

        // Assignment operators for other T
        Ptr<T>& operator=(const T & other)
        {
            *ptr_ = other;
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
        // Is this even useful?
        bool Unique() const {
            return ref_count_ && *ref_count_ == 1;
        }

        // get number of references to this pointer
        uint32_t Count() const {
            return *ref_count_;
        }

        // swap this pointer with other
        void Swap(Ptr<T>& other) {
            std::swap(ptr_, other.ptr_);
            std::swap(ref_count_, other.ref_count_);
        }

        // cast the stored pointer to type U and return it as raw pointer
        /*
        template <typename U>
        U * Value() const {
            return dynamic_cast<U*>(ptr_);
        }
        */

        // cast the stored pointer to type U and return it as Ptr<U>
        template <typename U>
        Ptr<U> Cast() const {
            return Ptr<U>(dynamic_cast<U*>(ptr_), ref_count_);
        }

        // check if T same type as U
        template<typename U>
        bool Same() const {
            return dynamic_cast<const U*>(ptr_) != nullptr;
        }

        // check if pointer initialized
        operator bool() const {
            return ptr_ != nullptr;
        }

        // check if two Ptrs same
        bool operator==(const Ptr<T>& lhs) {
            return lhs.ptr_ == ptr_;
        }

        // return demangled typename of T
        string Type(){
            const std::type_info& t = typeid(*ptr_);
            int status;
            char* demangled = abi::__cxa_demangle(t.name(), 0, 0, &status);
            string r(demangled);
            free(demangled);
            return r;
        }

        // Null reference
        static Ptr<T> Null;

private:

        // internal pointer
        T* ptr_;

        // reference count to number of copies of pointer
        uint32_t * ref_count_;

        // private constructor for dynamic_cast_to
        Ptr(T* ptr, uint32_t* ref_count) : ptr_(ptr), ref_count_(ref_count) {
            
            if (ptr_ != nullptr) {
                ++(*ref_count_);
            }

        }
        
        // allow Ptr<U> access to private constructor in Ptr<T>
        template <typename U>
        friend class Ptr;

    };

    // define static null value
    template<typename T>
    Ptr<T> Ptr<T>::Null = Ptr<T>();

#endif