#ifndef SAFE_PTR_H
#define SAFE_PTR_H
/**
 * @file SafePtr.h
 * @brief Header file for the SafePtr template class.
 *
 * This header file defines the `SafePtr` template class, which provides a smart pointer implementation.
 * It manages the lifetime of dynamically allocated memory and ensures proper memory management.
 *
 * @author Your Name (or Company)
 * @date 2024-07-09
 */

template<typename T>
/**
 * @brief A safe pointer template class.
 * 
 * @tparam T The type of elements stored in the list
 */
class SafePtr {
private:
  /**
   * @brief Raw pointer to the managed data.
   */
  T* ptr;

  /**
   * @brief Pointer to an unsigned integer that keeps track of the reference count for the managed memory.
   */
  unsigned int* refCount;

  /**
   * @brief Helper function to release the managed memory if the reference count reaches zero.
   */
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
  /**
   * @brief Default constructor that initializes both `ptr` and `refCount` to nullptr.
   */
  SafePtr() : ptr(nullptr), refCount(nullptr) {}

  /**
   * @brief Constructor that takes a raw pointer (`p`) as input.
   *
   * If `p` is not null, it creates a new `refCount` object initialized to 1 and assigns `p` to `ptr`.
   *
   * @param p The raw pointer to be managed.
   */
  explicit SafePtr(T* p) : ptr(p), refCount(p ? new unsigned int(1) : nullptr) {}

  /**
   * @brief Copy constructor that performs a deep copy of another `SafePtr` object.
   *
   * It increments the reference count of the shared resource if it exists.
   *
   * @param other The `SafePtr` object to be copied.
   */
  SafePtr(const SafePtr& other) : ptr(other.ptr), refCount(other.refCount) {
    if (refCount) {
      (*refCount)++;
    }
  }

  /**
   * @brief Copy assignment operator that performs a deep copy of another `SafePtr` object.
   *
   * It first releases the resources managed by the current object (if any)
   * and then assigns the `ptr` and `refCount` from the other object.
   * It also increments the reference count if the resource is not null.
   *
   * @param other The `SafePtr` object to be copied from.
   * @return A reference to the current `SafePtr` object.
   */
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

  /**
   * @brief Destructor that calls the `release()` function to ensure proper memory management.
   */
  ~SafePtr() {
    release();
  }

  /**
   * @brief Returns the raw pointer to the managed data.
   *
   * @return The raw pointer to the managed data (can be nullptr).
   */
  T* get() const { return ptr; }

  /**
   * @brief Dereference operator that returns the value pointed to by the managed data.
   *
   * It assumes the `ptr` is not null and throws an exception if it is.
   *
   * @return The value pointed to by the managed data.
   */
  T& operator*() const { return *ptr; }

  /**
   * @brief Arrow operator that returns the address of the value pointed to by the managed data.
   *
   * It assumes the `ptr` is not null and throws an exception if it is.
   *
   * @return The address of the value pointed to by the managed data.
   */

  T* operator->() const { return ptr; }

  /**
   * @brief Conversion operator to boolean.
   *
   * This operator allows using a `SafePtr` object in a boolean context.
   * It returns true if `ptr` is not null (meaning it points to valid data) and false otherwise.
   */
  operator bool() const { return ptr != nullptr; }
};

#endif // SAFE_PTR_H
   
