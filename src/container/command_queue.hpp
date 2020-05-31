#pragma once

/******************************************************************************
**  Includes
******************************************************************************/
#include "buffer_queue.hpp"

/******************************************************************************
**  Class Declaration
******************************************************************************/
namespace Gorilla
{
    class CommandQueue
    {
    public:
        CommandQueue  ();
        CommandQueue  (uint32_t capacity);
        CommandQueue  (const CommandQueue& queue);
        CommandQueue  (CommandQueue&& queue);
        ~CommandQueue ();

        inline uint32_t get_size        () const;
        inline uint32_t get_capacity    () const;
        inline bool     is_empty        () const;
        inline void     clear           ();

        template<typename TYPE, int(TYPE::*Callback)(void*), typename DATA>
        void            push(TYPE *object, const DATA& data);
        template<typename TYPE, int(TYPE::*Callback)(void)>
        void            push(TYPE *object);
        template<typename DATA>
        void            push(int(*callback)(void*), const DATA& data);
        void            push(int(*callback)(void));

        int             pop();

        inline void     swap            (CommandQueue& queue);
        inline void     reserve         (uint32_t capacity);

        inline void     operator=       (const CommandQueue& queue);
        inline void     operator=       (CommandQueue&& queue);

    private:
        enum class Format
        {
            CALLBACK = 0,
            CALLBACK_DATA,
            METHOD,
            METHOD_DATA,
        };

        struct CommandCallbackHeader
        {
            Format Format;
            int (*Callback)();
        };

        struct CommandCallbackDataHeader
        {
            Format Format;
            int (*Callback)(void *data);
        };

        struct CommandMethodHeader
        {
            Format Format;
            int (*Callback)(void *object);
            void* Object;
        };

        struct CommandMethodDataHeader
        {
            Format Format;
            int (*Callback)(void *object, void *data);
            void* Object;
        };

        void*           allocate(Format format, uint32_t data_size);

        template<typename TYPE, int(TYPE::*Callback)(void*)>
        static int      unpack_method_data(void *object, void *data);
        template<typename TYPE, int(TYPE::*Callback)(void)>
        static int      unpack_method(void *object);

    private:
        BufferQueue m_commands;
    };

    uint32_t CommandQueue::get_size() const
    {
        return m_commands.get_size();
    }

    uint32_t CommandQueue::get_capacity() const
    {
        return m_commands.get_capacity();
    }

    bool CommandQueue::is_empty() const
    {
        return m_commands.is_empty();
    }

    void CommandQueue::clear()
    {
        m_commands.clear();
    }

    template<typename TYPE, int(TYPE::*Callback)(void*)>
    int CommandQueue::unpack_method_data(void *object, void *data)
    {
        TYPE *tobject = (TYPE*)object;
        return (tobject->*Callback)(data);
    }

    template<typename TYPE, int(TYPE::*Callback)(void)>
    int CommandQueue::unpack_method(void *object)
    {
        TYPE *tobject = (TYPE*)object;
        return (tobject->*Callback)();
    }

    template<typename TYPE, int(TYPE::*Callback)(void*), typename DATA>
    void CommandQueue::push(TYPE *object, const DATA& data)
    {
        uint32_t command_size = sizeof(CommandMethodDataHeader) + sizeof(DATA);
        CommandMethodDataHeader *header = (CommandMethodDataHeader*)m_commands.push(command_size);
        header->Format = Format::METHOD_DATA;
        header->Callback = &unpack_method_data<TYPE, Callback>;
        header->Object = object;

        DATA& buffer = *(DATA*)(((uint8_t*)header) + sizeof(CommandMethodDataHeader));
        buffer = std::move(data);
    }

    template<typename TYPE, int(TYPE::*Callback)(void)>
    void CommandQueue::push(TYPE *object)
    {
        uint32_t command_size = sizeof(CommandMethodHeader);
        CommandMethodHeader *header = (CommandMethodHeader*)m_commands.push(command_size);
        header->Format = Format::METHOD;
        header->Callback = &unpack_method<TYPE, Callback>;
        header->Object = object;
    }

    template<typename DATA>
    void CommandQueue::push(int(*callback)(void*), const DATA& data)
    {
        uint32_t command_size = sizeof(CommandCallbackDataHeader) + sizeof(DATA);
        CommandCallbackDataHeader *header = (CommandCallbackDataHeader*)m_commands.push(command_size);
        header->Format = Format::CALLBACK_DATA;
        header->Callback = callback;

        DATA& buffer = *(DATA*)(((uint8_t*)header) + sizeof(CommandCallbackDataHeader));
        buffer = std::move(data);
    }

    void CommandQueue::swap(CommandQueue& queue)
    {
        m_commands.swap(queue.m_commands);
    }

    void CommandQueue::reserve(uint32_t capacity)
    {
        m_commands.reserve(capacity);
    }

    void CommandQueue::operator= (const CommandQueue& queue)
    {
        m_commands = queue.m_commands;
    }

    void CommandQueue::operator= (CommandQueue&& queue)
    {
        m_commands = queue.m_commands;
    }
}
