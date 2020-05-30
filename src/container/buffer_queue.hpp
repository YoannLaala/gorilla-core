#pragma once

/******************************************************************************
**  Includes
******************************************************************************/
#include "../memory/memory.hpp"

/******************************************************************************
**  Class Declaration
******************************************************************************/
namespace Gorilla
{
    class BufferQueue
    {
    public:
        BufferQueue  ();
        BufferQueue  (uint32_t capacity);
        BufferQueue  (const BufferQueue& queue);
        BufferQueue  (BufferQueue&& queue);
        ~BufferQueue ();

        inline uint32_t get_size        () const;
        inline uint32_t get_capacity    () const;
        inline bool     is_empty        () const;
        inline void     clear           ();

        void            push            (const void *data, uint32_t size);
        void*           push            (uint32_t size);
        void*           pop             ();

        void            swap            (BufferQueue& queue);
        void            reserve         (uint32_t capacity);

        inline void	    operator=       (const BufferQueue& queue);
        inline void     operator=       (BufferQueue&& queue);

    private:
        uint8_t*        m_buffer;
        uint32_t        m_capacity;
        uint32_t        m_start;
        uint32_t        m_end;
    };

    uint32_t BufferQueue::get_size() const
    {
        return m_end - m_start;
    }

    uint32_t BufferQueue::get_capacity() const
    {
        return m_capacity;
    }

    bool BufferQueue::is_empty() const
    {
        return m_start == m_end;
    }

    void BufferQueue::clear()
    {
        m_start = 0;
        m_end = 0;
    }
}
