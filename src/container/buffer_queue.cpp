/******************************************************************************
**  Includes
******************************************************************************/
#include "buffer_queue.hpp"

/******************************************************************************
**  Class Definition
******************************************************************************/
namespace Gorilla
{
    BufferQueue::BufferQueue()
        : m_buffer(nullptr)
        , m_capacity(0)
        , m_start(0)
        , m_end(0)
    {
        // Nothing to do
    }

    BufferQueue::BufferQueue(uint32_t capacity)
        : m_buffer(nullptr)
        , m_capacity(0)
        , m_start(0)
        , m_end(0)
    {
        reserve(capacity);
    }

    BufferQueue::BufferQueue(const BufferQueue& queue)
        : BufferQueue()
    {
        operator=(queue);
    }

    BufferQueue::BufferQueue(BufferQueue&& queue)
        : BufferQueue()
    {
        operator=(queue);
    }

    BufferQueue::~BufferQueue()
    {
        // Nothing to do
    }

    void BufferQueue::push(const void *data, uint32_t size)
    {
        void *buffer = push(size);
        MEMCPY(buffer, data, size);
    }

    void* BufferQueue::push(uint32_t size)
    {
        if (m_start == m_end)
            clear();

        // contains user data and data size
        uint32_t allocated_size = size + sizeof(uint32_t);
        uint32_t new_size = m_end + allocated_size;
        if (new_size > m_capacity)
            reserve(new_size << 1);

        uint32_t index = m_end;
        *(uint32_t*)&m_buffer[index] = size;
        m_end = new_size;

        return &m_buffer[index + sizeof(uint32_t)];
    }

    void* BufferQueue::pop()
    {
        if (m_start < m_end)
        {
            uint32_t index = m_start;
            uint32_t allocated_size = *(uint32_t*)&m_buffer[index] + sizeof(uint32_t);
            m_start += allocated_size;
            void *data = &m_buffer[index + sizeof(uint32_t)];

            return data;
        }

        return nullptr;
    }

    void BufferQueue::swap(BufferQueue& queue)
    {
        uint32_t start = m_start;
        uint32_t end = m_end;
        uint32_t capacity = m_capacity;
        uint8_t *buffer = m_buffer;

        m_start = queue.m_start;
        m_end = queue.m_end;
        m_capacity = queue.m_capacity;
        m_buffer = queue.m_buffer;

        queue.m_start = start;
        queue.m_end = end;
        queue.m_capacity = capacity;
        queue.m_buffer = buffer;
    }

    void BufferQueue::reserve(uint32_t capacity)
    {
        if (capacity > m_capacity)
        {
            uint8_t *new_buffer = new uint8_t[capacity];
            if (m_buffer)
            {
                MEMCPY(new_buffer, m_buffer, m_capacity);
                delete[] m_buffer;
            }

            m_buffer = new_buffer;
            m_capacity = capacity;
        }
    }

    void BufferQueue::operator= (const BufferQueue& queue)
    {
        m_start = queue.m_start;
        m_end = queue.m_end;
        m_capacity = queue.m_capacity;
        if (m_capacity)
        {
            if (m_buffer)
            {
                delete[] m_buffer;
                m_buffer = nullptr;
            }
            reserve(m_capacity);
            MEMCPY(m_buffer, queue.m_buffer, m_capacity);
        }
    }

    void BufferQueue::operator= (BufferQueue&& queue)
    {
        m_start = queue.m_start;
        m_end = queue.m_end;
        m_capacity = queue.m_capacity;
        m_buffer = queue.m_buffer;

        queue.m_start = 0;
        queue.m_end = 0;
        queue.m_capacity = 0;
        queue.m_buffer = nullptr;
    }
}
