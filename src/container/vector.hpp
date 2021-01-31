#pragma once

/******************************************************************************
**  Includes
******************************************************************************/
#include "../memory/memory.hpp"
#include <utility> // std::move
#include <cstdlib> // TODO: remove due to std::qsort

/******************************************************************************
**  Class Declaration
******************************************************************************/
namespace Gorilla
{
    template <typename TYPE>
    class Vector
    {
    public:
        Vector  ();
        Vector  (uint32_t size);
        Vector  (const Vector<TYPE> &values);
        Vector  (const Vector<TYPE> &&values);
        ~Vector	();

        inline uint32_t         get_size        () const;
        inline uint32_t         get_capacity    () const;
        inline bool             is_empty        () const;
        inline void             clear           ();

        inline TYPE&            get             (uint32_t index);
        inline const TYPE&      get             (uint32_t index) const;

        TYPE&                   add             ();
        TYPE&                   add             (const TYPE &value);
        void                    add             (const Vector<TYPE> &values);

        void                    insert          (uint32_t index, const TYPE &value);

        void                    remove          (const TYPE &value);
        void                    remove_index    (uint32_t index);

        uint32_t                find_index      (const TYPE &value) const;
        void                    sort            ();

        inline void             resize          (uint32_t size);
        void                    resize          (uint32_t size, const TYPE &value);

        void                    expand          (uint32_t size);
        void                    expand          (uint32_t size, const TYPE &value);

        void                    expand_to       (uint32_t index);
        void                    expand_to       (uint32_t index, const TYPE &value);

        bool                    reserve         (uint32_t capacity, bool copy = true);
        void                    set_buffer      (TYPE *array, uint32_t size, uint32_t capacity, bool heap);

        inline void	            operator=       (const Vector<TYPE> &values);
        inline void             operator=       (const Vector<TYPE> &&values);
        inline TYPE&            operator[]      (uint32_t index);
        inline const TYPE&      operator[]      (uint32_t index) const;

    private:
        TYPE*             m_array;
        uint32_t          m_capacity;
        uint32_t          m_size:31;    // Limit to 31bit to allow heap/stack
        uint32_t          m_heap:1;
    };

    //! @brief      Constructor
    template <typename TYPE>
    Vector<TYPE>::Vector()
        : m_array(nullptr)
        , m_capacity(0)
        , m_size(0)
        , m_heap(true)
    {
        // Nothing to do
    }

    //! @brief      Constructor
    //! @param      size is the number of element to allocate
    template <typename TYPE>
    Vector<TYPE>::Vector(uint32_t size)
        : Vector()
    {
        resize(size);
    }

    //! @brief      Constructor
    template <typename TYPE>
    Vector<TYPE>::Vector(const Vector<TYPE> &values)
        : Vector()
    {
        operator=(values);
    }

    //! @brief      Constructor
    template <typename TYPE>
    Vector<TYPE>::Vector(const Vector<TYPE> &&values)
        : Vector()
    {
        operator=(values);
    }

    //! @brief      Destructor
    template <typename TYPE>
    Vector<TYPE>::~Vector()
    {
        set_buffer(nullptr, 0, 0, true);
    }

    //! @brief      Add an element to the tail
    //! @return     Last element created
    template <typename TYPE>
    TYPE& Vector<TYPE>::add()
    {
        const uint32_t old_size = m_size;
        const uint32_t new_size = m_size + 1;
        if (new_size > m_capacity)
        {
            reserve(new_size << 1, true);
        }
        ++m_size;

        return m_array[old_size];
    }

    //! @brief      Add a specified element
    //! @params     value is the value to add to the vector
    //! @return     Last element created
    template <typename TYPE>
    TYPE& Vector<TYPE>::add(const TYPE &value)
    {
        TYPE &new_value = add();
        new_value = value;

        return new_value;
    }

    //! @brief      Add a complete Vector to the current one
    //! @params     values is the vector to add
    template <typename TYPE>
    void Vector<TYPE>::add(const Vector<TYPE> &values)
    {
        const uint32_t other_size = values.get_size();
        if (other_size == 0)
            return;

        const uint32_t new_size = m_size + other_size;
        if (new_size > m_capacity)
        {
            reserve(new_size << 1, true);
        }

        // Set all the value
        for (uint32_t element = 0; element < other_size; ++element)
        {
            m_array[m_size+element] = values[element];
        }
        m_size = new_size;
    }

    //! @brief      Insert an element
    //! @params     index is the offset where the value will be added
    //! @params     value is the value to be added at the specified index
    template <typename TYPE>
    void Vector<TYPE>::insert(uint32_t index, const TYPE &value)
    {
        // Last Element
        if (m_size <= index)
            expand_to(index);
        else
        {
            add();
            uint32_t bytes_to_move = (m_size - 1 - index) * sizeof(TYPE);
            if (bytes_to_move)
            {
                MEMMOVE(&m_array[index+1], &m_array[index], bytes_to_move);
            }
        }
        m_array[index] = value;
    }

    //! @brief      Remove an element
    //! @param      value is the element to remove
    template <typename TYPE>
    void Vector<TYPE>::remove(const TYPE &value)
    {
        uint32_t index = find(value);
        if (index != (uint32_t)-1)
        {
            remove_index(index);
        }
    }

    //! @brief      Remove element at a specific index
    //! @param      index is targeting the element to remove
    template <typename TYPE>
    void Vector<TYPE>::remove_index(uint32_t index)
    {
        if (index < m_size)
        {
            uint32_t last_index = --m_size;
            if (index != last_index)
            {
                m_array[index] = m_array[last_index];
            }
        }
    }

    //! @brief      Resize
    //! @param      size is the new size
    //! @details    Possibly lost all previous data, when allocating new array
    template <typename TYPE>
    void Vector<TYPE>::resize(uint32_t size)
    {
        reserve(size, false);
        m_size = size;
    }

    //! @brief      Resize
    //! @param      size is the new size
    //! @param      value is the default value apply to new elements created
    //! @details    Possibly lost all previous data, when allocating new array
    template <typename TYPE>
    void Vector<TYPE>::resize(uint32_t size, const TYPE &value)
    {
        reserve(size, false);

        // Set new default Value
        for (uint32_t element = 0; element < size; ++element)
        {
            m_array[element] = value;
        }
        m_size = size;
    }

    //! @brief      Expand
    //! @param      size is the number of element that will be added
    //! @details    Grow the array keeping the previous data
    template <typename TYPE>
    void Vector<TYPE>::expand(uint32_t size)
    {
        uint32_t new_size = m_size + size;
        if (new_size > m_capacity)
            reserve(new_size << 1, true);
        m_size = new_size;
    }

    //! @brief      Expand
    //! @param      size is the number of element that will be added
    //! @param      value is the default value apply to new elements created
    //! @details    Grow the array keeping the previous data
    template <typename TYPE>
    void Vector<TYPE>::expand(uint32_t size, const TYPE &value)
    {
        uint32_t old_size = size;
        uint32_t new_size = m_size + size;

        reserve(new_size << 1, true, true);

        // Set default Value
        for (uint32_t element = old_size; element < new_size; ++element)
        {
            m_array[element] = value;
        }
        m_size = new_size;
    }

    //! @brief      Expand to an index
    //! @param      index is defining the access wanted
    //! @details    Expand the array to allow access to the index
    template <typename TYPE>
    void Vector<TYPE>::expand_to(uint32_t index)
    {
        if (m_size <= index)
        {
            reserve(++index, true);
            m_size = index;
        }
    }

    //! @brief      Expand to an index
    //! @param      index is defining the access wanted
    //! @param      value is the default value apply to new elements created
    //! @details    Expand the array to allow access to the index
    template <typename TYPE>
    void Vector<TYPE>::expand_to(uint32_t index, const TYPE &value)
    {
        // Set default Value
        if (m_size <= index)
        {
            uint32_t old_size = m_size;
            reserve(++index, true);
            for (uint32_t element = old_size; element < index; ++element)
            {
                m_array[element] = value;
            }
            m_size = index;
        }
    }

    //! @brief      Alocate memory upfront
    //! @params     capacity is the number element to allocate
    //! @params     copy is defining if the previous data will be kept
    template <typename TYPE>
    bool Vector<TYPE>::reserve(uint32_t capacity, bool copy /*= true*/)
    {
        if (capacity > m_capacity)
        {
            // Allocate new Array
            TYPE *new_array = new TYPE[capacity];

            // Copy old Element
            if (copy && m_array != nullptr)
            {
                Memory::memcpy<TYPE>(new_array, m_array, m_size);
            }

            set_buffer(new_array, m_size, capacity, true);
            return true;
        }

        return false;
    }

    //! @brief      operator=
    //! @details    Handle operator of other class that container (Force to copy data, not array pointer)
    template <typename TYPE>
    void Vector<TYPE>::operator= (const Vector<TYPE> &values)
    {
        m_size = values.get_size();
        if (m_size)
        {
            reserve(m_size, false);
            Memory::memcpy<TYPE>(m_array, values.m_array, m_size);
        }
    }

    //! @brief      operator=
    //! @details    Handle operator of other class that container (Force to copy data, not array pointer)
    template <typename TYPE>
    void Vector<TYPE>::operator= (const Vector<TYPE> &&values)
    {
        clear();

        m_size = values.get_size();
        if (m_size)
        {
            m_capacity = values.get_capacity();
            m_array = values.m_array;
            m_heap = values.m_heap;
            const_cast<Vector<TYPE>*>(&values)->m_array = nullptr;
        }
    }

    //! @brief      operator[]
    template <typename TYPE>
    TYPE& Vector<TYPE>::operator[](uint32_t index)
    {
        return m_array[index];
    }

    //! @brief      operator[]
    template <typename TYPE>
    const TYPE& Vector<TYPE>::operator[](uint32_t index) const
    {
        return m_array[index];
    }

    //! @brief      Get an element
    template <typename TYPE>
    TYPE& Vector<TYPE>::get(uint32_t index)
    {
        return m_array[index];
    }

    //! @brief      Get an element
    template <typename TYPE>
    const TYPE& Vector<TYPE>::get(uint32_t index) const
    {
        return m_array[index];
    }

    //! @brief      Get the number of element
    template <typename TYPE>
    uint32_t Vector<TYPE>::get_size() const
    {
        return m_size;
    }

    //! @brief      Get the number of element allocated
    template <typename TYPE>
    uint32_t Vector<TYPE>::get_capacity() const
    {
        return m_capacity;
    }

    //! @brief      Check if there is element
    template <typename TYPE>
    bool Vector<TYPE>::is_empty() const
    {
        return m_size == 0;
    }

    //! @brief      Clear
    template <typename TYPE>
    void Vector<TYPE>::clear()
    {
        m_size = 0;
    }

    //! @brief      Find index of an element
    template <typename TYPE>
    uint32_t Vector<TYPE>::find_index(const TYPE &value) const
    {
        for (uint32_t element = 0; element < m_size; ++element)
        {
            if (m_array[element] == value)
            {
                return element;
            }
        }

        return (uint32_t)-1;
    }

    //! @brief      Compare
    //! @details    qsort compare implementation between 2 elements
    template <typename TYPE>
    int compare(const void *value_a, const void *value_b)
    {
        if (*(TYPE*)value_a < *(TYPE*)value_b)
        {
            return -1;
        }

        if (*(TYPE*)value_a > *(TYPE*)value_b)
        {
            return 1;
        }

        return 0;
    }

    //! @brief      Sort
    //! @details    Based on qsort: elements need to implements "<" & ">" operators
    template <typename TYPE>
    void Vector<TYPE>::sort()
    {
        std::qsort(m_array, m_size, sizeof(TYPE), &compare<TYPE>);
    }

    //! @brief      Set internal buffer representing elements
    template <typename TYPE>
    void Vector<TYPE>::set_buffer(TYPE *array, uint32_t size, uint32_t capacity, bool heap)
    {
        // Delete heap array
        if (m_array && m_heap)
            delete[] m_array;

        m_array = array;
        m_size = size;
        m_capacity = capacity;
        m_heap = heap;
    }
}
