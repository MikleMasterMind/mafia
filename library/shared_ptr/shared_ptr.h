#pragma once

#include <cstddef>
#include <utility>

template <typename T>
class SharedPtr {
public:
    SharedPtr() noexcept
        : ptr_(nullptr)
        , ref_count_(nullptr)
    {
    }

    explicit SharedPtr(T* ptr)
        : ptr_(ptr)
        , ref_count_(ptr ? new size_t(1) : nullptr)
    {
    }

    SharedPtr(const SharedPtr& other) noexcept
        : ptr_(other.ptr_)
        , ref_count_(other.ref_count_)
    {
        if (ref_count_) {
            ++(*ref_count_);
        }
    }

    SharedPtr(SharedPtr&& other) noexcept
        : ptr_(other.ptr_)
        , ref_count_(other.ref_count_)
    {
        other.ptr_ = nullptr;
        other.ref_count_ = nullptr;
    }

    ~SharedPtr() {
        release();
    }

    SharedPtr& operator=(const SharedPtr& other) noexcept {
        if (this != &other) {
            release();
            ptr_ = other.ptr_;
            ref_count_ = other.ref_count_;
            if (ref_count_) {
                ++(*ref_count_);
            }
        }
        return *this;
    }

    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            release();
            ptr_ = other.ptr_;
            ref_count_ = other.ref_count_;
            other.ptr_ = nullptr;
            other.ref_count_ = nullptr;
        }
        return *this;
    }

    T& operator*() const noexcept {
        return *ptr_;
    }

    T* operator->() const noexcept {
        return ptr_;
    }

    T* get() const noexcept {
        return ptr_;
    }

    explicit operator bool() const noexcept {
        return ptr_ != nullptr;
    }

    size_t use_count() const noexcept {
        return ref_count_ ? *ref_count_ : 0;
    }

    void reset() noexcept {
        release();
        ptr_ = nullptr;
        ref_count_ = nullptr;
    }

    void reset(T* ptr) {
        if (ptr_ == ptr) {
            return;
        }
        release();
        ptr_ = ptr;
        ref_count_ = ptr ? new size_t(1) : nullptr;
    }

    void swap(SharedPtr& other) noexcept {
        using std::swap;
        swap(ptr_, other.ptr_);
        swap(ref_count_, other.ref_count_);
    }

private:
    void release() {
        if (ref_count_) {
            --(*ref_count_);
            if (*ref_count_ == 0) {
                delete ptr_;
                delete ref_count_;
            }
        }
    }

private:
    T* ptr_;
    size_t* ref_count_;
};

template <typename T, typename U>
bool operator==(const SharedPtr<T>& lhs, const SharedPtr<U>& rhs) {
    return lhs.get() == rhs.get();
}

template <typename T, typename U>
bool operator!=(const SharedPtr<T>& lhs, const SharedPtr<U>& rhs) {
    return !(lhs == rhs);
}

template <typename T>
bool operator==(const SharedPtr<T>& lhs, std::nullptr_t) {
    return lhs.get() == nullptr;
}

template <typename T>
bool operator==(std::nullptr_t, const SharedPtr<T>& rhs) {
    return rhs.get() == nullptr;
}

template <typename T>
bool operator!=(const SharedPtr<T>& lhs, std::nullptr_t) {
    return lhs.get() != nullptr;
}

template <typename T>
bool operator!=(std::nullptr_t, const SharedPtr<T>& rhs) {
    return rhs.get() != nullptr;
}

template <typename T, typename U>
bool operator<(const SharedPtr<T>& lhs, const SharedPtr<U>& rhs) {
    return lhs.get() < rhs.get();
}

template <typename T, typename U>
bool operator>(const SharedPtr<T>& lhs, const SharedPtr<U>& rhs) {
    return rhs < lhs;
}

template <typename T, typename U>
bool operator<=(const SharedPtr<T>& lhs, const SharedPtr<U>& rhs) {
    return !(rhs < lhs);
}

template <typename T, typename U>
bool operator>=(const SharedPtr<T>& lhs, const SharedPtr<U>& rhs) {
    return !(lhs < rhs);
}

template <typename T>
void swap(SharedPtr<T>& lhs, SharedPtr<T>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename T, typename... Args>
SharedPtr<T> make_shared(Args&&... args) {
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}
