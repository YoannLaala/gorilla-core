#pragma once

/******************************************************************************
**  Includes
******************************************************************************/
#include "memory/memory.hpp"

/******************************************************************************
**  Class Definition
******************************************************************************/
namespace Gorilla
{
    template <class TYPE>
    class Singleton
    {
    public:
        static inline void  create();
        static inline void  destroy();
        static inline TYPE* get_instance();
        static inline bool  is_created();
        static inline void  set_instance(TYPE *instance);

    protected:
        Singleton(){}

    protected:
        static TYPE *_instance;
    };

    template <class TYPE>
    TYPE *Singleton<TYPE>::_instance = nullptr;

    //! @brief      Create a Singleton of the specified class
    template <class TYPE>
    void Singleton<TYPE>::create()
    {
        LOG_ASSERT(!_instance, "Instance should be nullpr");
        _instance = new TYPE();
    }

    //! @brief      Destroy the Singleton of the specified class
    template <class TYPE>
    void Singleton<TYPE>::destroy()
    {
        delete _instance;
        _instance = nullptr;
    }

    //! @brief      Retrieve the Singleton of the specified class
    //! @return     the instance itself
    template <class TYPE>
    TYPE* Singleton<TYPE>::get_instance()
    {
        return _instance;
    }

    //! @brief      Check if the Singleton of the specified class has already been created
    //! @return     the instance has been created
    template <class TYPE>
    bool Singleton<TYPE>::is_created()
    {
        return _instance != nullptr;
    }

    //! @brief      Propagate Singleton amond shared libarry
    //! @param      instance is the instance to set as singleton
    template <class TYPE>
    void Singleton<TYPE>::set_instance(TYPE *instance)
    {
        LOG_ASSERT(!_instance, "Instance should be nullpr");
        _instance = instance;
    }
}
