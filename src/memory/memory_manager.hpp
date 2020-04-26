#pragma once

/******************************************************************************
**  Includes
******************************************************************************/
#include "../container/vector.hpp"
#include "../thread/thread.hpp"

/******************************************************************************
**  Class Declaration
******************************************************************************/
namespace Gorilla
{
    struct MemoryStatistics
    {
        uint64_t memory_allocated;
        uint64_t memory_used;
        uint64_t page_allocated;
        uint64_t allocation_used;
        uint64_t allocation_unused;
        Vector<uint64_t> domains_allocated;
    };

    class MemoryManager
    {
    public:
        inline static MemoryManager*    get_instance        ();

        uint32_t                        get_page_size       () const;
        void                            set_page_size       (uint32_t size);

        void*                           allocate            (uint64_t size);
        void                            deallocate          (void *address);

        void                            set_domain          (uint32_t domain);

        void                            get_statistics      (MemoryStatistics *statistics);

    private:
        MemoryManager();
        ~MemoryManager();

    private:
        static THREAD_LOCAL uint32_t      m_domain;
        uint32_t                          m_default_page_size;
        MemoryStatistics                m_statistics;
        Vector<void*>                   m_pages;
        Vector<void*>                   m_frees;
    };

    //! @brief      Get the instance
    MemoryManager* MemoryManager::get_instance()
    {
        static MemoryManager memory_manager;
        return &memory_manager;
    }
}

//#define MEMORY_DOMAIN(_id_) MemoryManager::GetInstance()->SetDomain(_id_, #_id_)
//#ifdef MEMORY_MANAGER_ACTIVATED
//inline void* operator new(size_t _uiSize) { return Gorilla::MemoryManager::GetInstance()->Allocate(_uiSize);	}
//inline void* operator new[](size_t _uiSize) { return Gorilla::MemoryManager::GetInstance()->Allocate(_uiSize); }
//inline void operator delete(void* _pValue) { return Gorilla::MemoryManager::GetInstance()->Deallocate(_pValue); }
//inline void operator delete[](void* _pValue) { return Gorilla::MemoryManager::GetInstance()->Deallocate(_pValue); }
//#endif
