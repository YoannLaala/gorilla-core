/******************************************************************************
**  Includes
******************************************************************************/
#include "command_queue.hpp"

/******************************************************************************
**  Class Definition
******************************************************************************/
namespace Gorilla
{
    CommandQueue::CommandQueue()
    {
        // Nothing to do
    }

    CommandQueue::CommandQueue(uint32_t capacity)
        : m_commands(capacity)
    {
        reserve(capacity);
    }

    CommandQueue::CommandQueue(const CommandQueue& queue)
        : CommandQueue()
    {
        operator=(queue);
    }

    CommandQueue::CommandQueue(CommandQueue&& queue)
        : CommandQueue()
    {
        operator=(queue);
    }

    CommandQueue::~CommandQueue()
    {
        // Nothing to do
    }

    void CommandQueue::push(int(*callback)(void))
    {
        uint32_t command_size = sizeof(CommandCallbackHeader);
        CommandCallbackHeader *header = (CommandCallbackHeader*)m_commands.push(command_size);
        header->Format = Format::CALLBACK;
        header->Callback = callback;
    }

    int CommandQueue::pop()
    {
        int error = 0;

        void *buffer = m_commands.pop();
        if (buffer)
        {
            switch (*(Format*)buffer)
            {
                case Format::CALLBACK:
                {
                    CommandCallbackHeader *header = (CommandCallbackHeader*)buffer;
                    error = header->Callback();
                    break;
                }

                case Format::CALLBACK_DATA:
                {
                    CommandCallbackDataHeader *header = (CommandCallbackDataHeader*)buffer;
                    void *data = ((uint8_t*)buffer) + sizeof(CommandCallbackDataHeader);
                    error = header->Callback(data);
                    break;
                }

                case Format::METHOD:
                {
                    CommandMethodHeader *header = (CommandMethodHeader*)buffer;
                    error = header->Callback(header->Object);
                    break;
                }

                case Format::METHOD_DATA:
                {
                    CommandMethodDataHeader *header = (CommandMethodDataHeader*)buffer;
                    void *data = ((uint8_t*)buffer) + sizeof(CommandMethodDataHeader);
                    error = header->Callback(header->Object, data);;
                    break;
                }
            }
        }

        return error;
    }
}
