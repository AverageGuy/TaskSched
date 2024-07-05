#ifndef SAFE_PTR_H
#define SAFE_PTR_H

template<typename T>
class SafePtr {
private:
    T* ptr;
    unsigned int* refCount;

    void release() {
        if (refCount) {
            (*refCount)--;
            if (*refCount == 0) {
                delete ptr;
                delete refCount;
            }
        }
    }

public:
    SafePtr() : ptr(nullptr), refCount(nullptr) {}

    explicit SafePtr(T* p) : ptr(p), refCount(p ? new unsigned int(1) : nullptr) {}

    SafePtr(const SafePtr& other) : ptr(other.ptr), refCount(other.refCount) {
        if (refCount) {
            (*refCount)++;
        }
    }

    SafePtr& operator=(const SafePtr& other) {
        if (this != &other) {
            release();
            ptr = other.ptr;
            refCount = other.refCount;
            if (refCount) {
                (*refCount)++;
            }
        }
        return *this;
    }

    ~SafePtr() {
        release();
    }

    T* get() const { return ptr; }
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }

    operator bool() const { return ptr != nullptr; }
};

#endif // SAFE_PTR_H
