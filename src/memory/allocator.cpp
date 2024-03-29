
#include "memory/allocator.h"

namespace nk {
    Allocator::Allocator(Allocator&& other)
        : m_size_bytes{other.m_size_bytes},
          m_used_bytes{other.m_used_bytes},
          m_allocation_count{other.m_allocation_count},
#if !defined(NK_RELEASE)
          m_start{other.m_start},
          m_name{other.m_name},
          m_type{other.m_type}
#else
          m_start{other.m_start}
#endif
    {
        other.m_size_bytes = 0;
        other.m_used_bytes = 0;
        other.m_allocation_count = 0;
        other.m_start = nullptr;
#if !defined(NK_RELEASE)
        other.m_name = "None";
        other.m_type = MemoryType::None;
#endif
    }

    Allocator& Allocator::operator=(Allocator&& rhs) {
        m_size_bytes = rhs.m_size_bytes;
        m_used_bytes = rhs.m_used_bytes;
        m_allocation_count = rhs.m_allocation_count;
        m_start = rhs.m_start;
#if !defined(NK_RELEASE)
        m_name = rhs.m_name;
        m_type = rhs.m_type;
#endif

        rhs.m_size_bytes = 0;
        rhs.m_used_bytes = 0;
        rhs.m_allocation_count = 0;
        rhs.m_start = nullptr;
#if !defined(NK_RELEASE)
        rhs.m_name = "None";
        rhs.m_type = MemoryType::None;
#endif

        return *this;
    }

    Allocator::~Allocator() {
#if !defined(NK_RELEASE)
        if (m_allocation_count != 0 || m_used_bytes != 0) {
            FatalLog("{} Allocator not correctly freed!", m_name);
            MemoryManagerReport();
        }
#endif
        Assert(m_allocation_count == 0 && m_used_bytes == 0);
    }
}

