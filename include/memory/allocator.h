#pragma once

#include "memory/memory_type.h"
#include "core/result.h"

namespace nk {
    class Allocator {
    public:
        Allocator()
            : m_size_bytes{0},
              m_used_bytes{0},
              m_allocation_count{0},
              m_start{nullptr} {}

        Allocator(const Allocator&) = delete;
        Allocator& operator=(Allocator&) = delete;

        Allocator(Allocator&&);
        Allocator& operator=(Allocator&&);

        virtual ~Allocator();

        template <typename T>
        inline T* allocate() {
            return static_cast<T*>(allocate_impl(sizeof(T), alignof(T)));
        }

        template <typename T>
        inline T* allocate_lot(const u64 lot) {
            return static_cast<T*>(allocate_impl(sizeof(T) * lot, alignof(T)));
        }

        template <typename T>
        inline void free(T* ptr) {
            free_impl(ptr, sizeof(T));
        }

        template <typename T>
        inline void free_lot(T* ptr, const u64 lot) {
            free_impl(ptr, sizeof(T) * lot);
        }

        template <typename T, typename... Args>
        inline T* construct(Args&&... args) {
            return new (allocate_impl(sizeof(T), alignof(T))) T(std::forward<Args>(args)...);
        }

        template <typename T, typename V>
        inline void destroy(V* ptr) {
            if (ptr == nullptr) {
                return;
            }

            ptr->~V();
            free_impl(ptr, sizeof(T));
        }

        inline u64 size() const { return m_size_bytes; }
        inline u64 used() const { return m_used_bytes; }
        inline u64 allocation_count() const { return m_allocation_count; }
        inline const void* start() const { return m_start; }

        virtual void* allocate_impl(const u64 size_bytes, const u64 alignment) = 0;
        virtual void free_impl(void* const ptr, const u64 size_bytes) = 0;

    protected:
        void init(const u64 size_bytes, void* const start);
#if !defined(NK_RELEASE)
        void init(const u64 size_bytes, void* const start, str name, const MemoryTypeValue memory_type);
#endif

        u64 m_size_bytes;
        u64 m_used_bytes;
        u64 m_allocation_count;

        void* m_start;

#if !defined(NK_RELEASE)
        str m_name;
        MemoryTypeValue m_type;
#endif
    };

#if !defined(NK_RELEASE)
    #define AllocatorInit(size_bytes, start, name, memory_type) \
        init(size_bytes, start, name, memory_type)
#else
    #define AllocatorInit(size_bytes, start, name, memory_type) \
        init(size_bytes, start)
#endif
}
