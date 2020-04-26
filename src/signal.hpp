#pragma once

/******************************************************************************
**  Includes
******************************************************************************/
#include "container/vector.hpp"

namespace Gorilla
{
    template <typename... ARGUMENTS>
    class Signal
    {
    private:
        template<typename TYPE>
        struct TReceiver
        {
            typedef void (TYPE::*MemberFunction)(ARGUMENTS... arguments);
            static void bridge(void *receiver, ARGUMENTS... arguments)
            {
                TReceiver<TYPE> *treceiver = static_cast<TReceiver<TYPE>*>(receiver);
                (treceiver->object->*treceiver->function)(arguments...);
            }

            void (*callback)(void *receiver, ARGUMENTS... arguments);
            TYPE *object;
            MemberFunction function;
        };

    public:
        template <typename TYPE>
        void    connect             (TYPE *object, typename TReceiver<TYPE>::MemberFunction member_function);
        void    disconnect          (void *object);
        void    disconnect_all      ();
        void    send                (ARGUMENTS... arguments);

    private:
        uint32_t  get_receiver_index  (void* object);

    private:
        struct Receiver
        {
            void (*callback)(void *receiver, ARGUMENTS... arguments);
            void *object;
            void *function;
        };
        Vector<Receiver> m_receivers;
    };

    //! @brief      Connect an object to listen
    //! @param      object is an instance to listen signals
    //! @param      member_function is a pointer on member function to call when signal is send
    template <typename... ARGUMENTS>
    template <typename TYPE>
    void Signal<ARGUMENTS...>::connect(TYPE *object, typename TReceiver<TYPE>::MemberFunction member_function)
    {
        if(get_receiver_index(object) == (uint32_t)-1)
        {
            TReceiver<TYPE> *treceiver = reinterpret_cast<TReceiver<TYPE>*>(&m_receivers.add());
            treceiver->object = object;
            treceiver->function = member_function;
            treceiver->callback = &TReceiver<TYPE>::bridge;
        }
    }

    //! @brief      Disconnect an object to the signal
    //! @param      object is an instance which was listening
    template <typename... ARGUMENTS>
    void Signal<ARGUMENTS...>::disconnect(void *object)
    {
        uint32_t index = get_receiver_index(object);
        if (index != (uint32_t)-1)
            m_receivers.remove_index(index);
    }

    //! @brief      Disconnect all objects on this signal
    template <typename... ARGUMENTS>
    void Signal<ARGUMENTS...>::disconnect_all()
    {
        m_receivers.clear();
    }

    //! @brief      Send a signal to connected objects
    //! @param      arguments are variadic values send to connected objects
    template <typename... ARGUMENTS>
    void Signal<ARGUMENTS...>::send(ARGUMENTS... arguments)
    {
        const uint32_t receiver_count = m_receivers.get_size();
        for (uint32_t i = 0; i < receiver_count; ++i)
        {
            Receiver *receiver = &m_receivers[i];
            receiver->callback(receiver, arguments...);
        }
    }

    //! @brief      Get index of the object in current vector
    //! @param      object is the instance to look for
    //! @return     index in vector where is object is
    template <typename... ARGUMENTS>
    uint32_t Signal<ARGUMENTS...>::get_receiver_index(void *object)
    {
       const uint32_t receiver_count = m_receivers.get_size();
        for (uint32_t i = 0; i < receiver_count; ++i)
        {
            const Receiver& receiver = m_receivers[i];
            if (receiver.object  == object)
                return i;
        }

        return (uint32_t)-1;
    }
}
