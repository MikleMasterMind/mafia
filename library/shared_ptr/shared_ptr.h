#pragma once

#include <cstddef>
#include <utility>
#include <mutex>
#include <atomic>
#include <memory>

namespace NMafia {
    template <typename T>
    class TSharedPtr {
    public:
        TSharedPtr() noexcept
            : ptr_(nullptr)
            , ref_count_(nullptr)
        {
        }

        explicit TSharedPtr(T* ptr)
            : ptr_(ptr)
            , ref_count_(ptr ? new std::atomic<size_t>(1) : nullptr)
        {
        }

        TSharedPtr(const TSharedPtr& other) noexcept {
            std::lock_guard<std::mutex> lock(other.mutex_);
            ptr_ = other.ptr_;
            ref_count_ = other.ref_count_;
            if (ref_count_) {
                ++(*ref_count_);
            }
        }

        ~TSharedPtr() noexcept {
            release();
        }

        TSharedPtr& operator=(const TSharedPtr& other) noexcept {
            if (this != &other) {
                TSharedPtr temp(other);
                swap(temp);
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
            if (ptr_ != ptr) {
                release();
                ptr_ = ptr;
                ref_count_ = ptr ? new std::atomic<size_t>(1) : nullptr;
            }
        }

        void swap(TSharedPtr& other) noexcept {
            std::swap(ptr_, other.ptr_);
            std::swap(ref_count_, other.ref_count_);
        }

    private:
        void release() {
            if (ref_count_) {
                if (--(*ref_count_) == 0) {
                    delete ptr_;
                    delete ref_count_;
                    ptr_ = nullptr;
                    ref_count_ = nullptr;
                }
            }
        }

    private:
        T* ptr_;
        std::atomic<size_t>* ref_count_;
        mutable std::mutex mutex_;
    };

    template <typename T, typename U>
    bool operator==(const TSharedPtr<T>& lhs, const TSharedPtr<U>& rhs) {
        return lhs.get() == rhs.get();
    }

    template <typename T, typename U>
    bool operator!=(const TSharedPtr<T>& lhs, const TSharedPtr<U>& rhs) {
        return !(lhs == rhs);
    }

    template <typename T>
    bool operator==(const TSharedPtr<T>& lhs, std::nullptr_t) {
        return lhs.get() == nullptr;
    }

    template <typename T>
    bool operator==(std::nullptr_t, const TSharedPtr<T>& rhs) {
        return rhs.get() == nullptr;
    }

    template <typename T>
    bool operator!=(const TSharedPtr<T>& lhs, std::nullptr_t) {
        return lhs.get() != nullptr;
    }

    template <typename T>
    bool operator!=(std::nullptr_t, const TSharedPtr<T>& rhs) {
        return rhs.get() != nullptr;
    }

    // Функция создания shared_ptr
    template <typename T, typename... Args>
    TSharedPtr<T> make_shared(Args&&... args) {
        return TSharedPtr<T>(new T(std::forward<Args>(args)...));
    }
}
