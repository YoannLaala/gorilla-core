/******************************************************************************
**  Includes
******************************************************************************/
#include "memory_manager.hpp"

/******************************************************************************
**  Defines
******************************************************************************/
#define INITIAL_PAGE_COUNT 1
#define INITIAL_PAGE_SIZE 128*1024

/******************************************************************************
**  Class Definition
******************************************************************************/

//#ifdef MEMORY_MANAGER_ACTIVATED
//Gorilla::MemoryManager*Gorilla:: MemoryManager::m_pInstance = new (Gorilla::Memory::Allocate(sizeof(Gorilla::MemoryManager))) Gorilla::MemoryManager();
//#else
//Gorilla::MemoryManager* Gorilla::MemoryManager::m_pInstance = NULL;
//#endif

namespace Gorilla
{
    struct PageHeader
    {
        uint64_t allocation_count;
    };

    struct AllocationHeader
    {
        uint8_t domain;
        uint32_t page_index;
        uint64_t size;
    };

    THREAD_LOCAL uint32_t MemoryManager::m_domain = 0;

    //! @brief      Constructor
    MemoryManager::MemoryManager()
        : m_default_page_size(INITIAL_PAGE_SIZE)
    {
        // allocate pages vector
        uint8_t* pages = (uint8_t*)Memory::allocate(INITIAL_PAGE_COUNT * sizeof(void*));
        m_pages.set_buffer((void**)pages, 0, INITIAL_PAGE_COUNT, true);

        MEMSET(&m_statistics, 0, sizeof(m_statistics));

        // allocate domains vector
        uint64_t* domains = (uint64_t*)Memory::allocate(INITIAL_PAGE_COUNT * sizeof(uint64_t));
        MEMSET(domains, 0, INITIAL_PAGE_COUNT * sizeof(uint64_t));
        m_statistics.domains_allocated.set_buffer(domains, 1, INITIAL_PAGE_COUNT, true);
    }

    //! @brief      Destructor
    MemoryManager::~MemoryManager()
    {
        // check
    }

    //! @brief      Allocate
    //! @param      size is the size of the allocation
    //! @details    Allocate memory add update current MemoryDomain
    void* MemoryManager::allocate(uint64_t size)
    {
        uint32_t header_index = (uint32_t)-1;

        // search in free to avoid fragmentation
        uint64_t allocation_size = sizeof(AllocationHeader) + size;
        const uint32_t free_acount = m_frees.get_size();
        for (uint32_t i = 0; i < free_acount; ++i)
        {
            AllocationHeader *header = (AllocationHeader*)m_frees[i];
            if (allocation_size <= header->size)
            {
                header_index = i;
                break;
            }
        }

        // create a new page
        AllocationHeader *header = nullptr;
        if (header_index != (uint32_t)-1)
        {
            header = (AllocationHeader*)m_frees[header_index];
        }
        else if (header_index == (uint32_t)-1)
        {
            uint64_t page_size = sizeof(PageHeader) + allocation_size;
            uint64_t allocated_page_size = page_size < m_default_page_size ? m_default_page_size : page_size;
            PageHeader *page_header = (PageHeader*)Memory::allocate(allocated_page_size);
            page_header->allocation_count = 0;
            uint32_t page_index = m_pages.get_size();
            m_pages.add(page_header);

            header = (AllocationHeader*)((uint8_t*)page_header + sizeof(PageHeader));
            header->page_index = page_index;
            header->size = allocated_page_size - sizeof(PageHeader);
            if (allocation_size < m_default_page_size)
            {
                header_index = m_frees.get_size();
                m_frees.add(header);

                ++m_statistics.allocation_unused;
            }

            m_statistics.memory_allocated += allocated_page_size;
            m_statistics.memory_used += sizeof(PageHeader);
            ++m_statistics.page_allocated;
        }

        void *address = ((uint8_t*)header) + sizeof(AllocationHeader);

        // available header
        if (header_index != (uint32_t)-1)
        {
            // not enough space so keep it with this allocation
            uint64_t next_allocation_size = header->size - allocation_size;
            if (next_allocation_size <= sizeof(AllocationHeader))
            {
                allocation_size += next_allocation_size;
                m_frees.remove_index(header_index);

                --m_statistics.allocation_unused;
            }
            else
            {
                AllocationHeader *next_header = (AllocationHeader*)(((uint8_t*)header) + allocation_size);
                next_header->page_index = header->page_index;
                next_header->size = next_allocation_size;
                m_frees[header_index] = next_header;
            }
        }

        PageHeader *page_header = (PageHeader*)m_pages[header->page_index];
        ++page_header->allocation_count;

        header->domain = m_domain;
        header->size = allocation_size;

        ++m_statistics.allocation_used;
        m_statistics.memory_used += allocation_size;
        m_statistics.domains_allocated[header->domain] += allocation_size;

        return address;
    }

    //! @brief      Deallocate
    //! @details    Deallocate the memory and update the MemoryDomain associated
    void MemoryManager::deallocate(void *address)
    {
        AllocationHeader *allocation_header = (AllocationHeader*)(((uint8_t*)address) - sizeof(AllocationHeader));
        PageHeader *page_header = (PageHeader*)m_pages[allocation_header->page_index];

        --m_statistics.allocation_used;
        m_statistics.memory_used -= allocation_header->size;
        m_statistics.domains_allocated[allocation_header->domain] += allocation_header->size;

        const int32_t free_acount = (int32_t)m_frees.get_size();
        if (--page_header->allocation_count == 0)
        {
            for (int32_t i = free_acount-1; i >= 0; --i)
            {
                AllocationHeader *free_header = (AllocationHeader*)m_frees[i];
                if (free_header->page_index == allocation_header->page_index)
                {
                    m_frees.remove_index(i);

                    --m_statistics.allocation_unused;
                    m_statistics.memory_allocated -= free_header->size;
                }
            }

            --m_statistics.page_allocated;
            m_statistics.memory_used -= sizeof(PageHeader);
            m_statistics.memory_allocated -= sizeof(PageHeader) + allocation_header->size;

            m_pages.remove_index(allocation_header->page_index);
            Memory::deallocate(page_header);
        }
        else
        {
            bool header_reused = false;
            for (int32_t i = 0; i < free_acount; ++i)
            {
                AllocationHeader *free_header = (AllocationHeader*)m_frees[i];

                // merge previous + current
                if (((uint8_t*)free_header) + free_header->size == (uint8_t*)allocation_header)
                {
                    free_header->size += allocation_header->size;
                    header_reused = true;
                    break;
                }

                // merge current + next
                if (((uint8_t*)allocation_header) + allocation_header->size == (uint8_t*)free_header)
                {
                    allocation_header->size += allocation_header->size;
                    m_frees[i] = allocation_header;
                    header_reused = true;
                    break;
                }
            }

            if (!header_reused)
            {
                m_frees.add(allocation_header);

                ++m_statistics.allocation_unused;
            }
        }
    }

    //! @brief      get_page_size
    //! @details    Retrieve the size of for physical pages
    uint32_t MemoryManager::get_page_size() const
    {
        return m_default_page_size;
    }

    //! @brief      set_page_size
    //! @details    Set the current physical page size
    void MemoryManager::set_page_size(uint32_t size)
    {
        m_default_page_size = size;
    }

    //! @brief      Affect the current memory domain
    //! @details    Set the current domain for the next allocation
    void MemoryManager::set_domain(uint32_t domain)
    {
        m_domain = domain;
        m_statistics.domains_allocated.expand_to(domain, 0);
    }


    //! @brief      get_statistics
    //! @details    Retrieve the statistics abour the current memory usage
    void MemoryManager::get_statistics(MemoryStatistics *statistics)
    {
        *statistics = m_statistics;
    }
}
