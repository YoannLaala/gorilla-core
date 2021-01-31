#pragma once

/******************************************************************************
**  Includes
******************************************************************************/
#include "../memory/memory.hpp"
#include "../hash.hpp"
#include "vector.hpp"

/******************************************************************************
**  Defines
******************************************************************************/
#define HASH_MAP_STACK_CAPACITY     32
#define HASH_MAP_BLOCK_ALLOCATION   1

/******************************************************************************
**  Class Declaration
******************************************************************************/
namespace Gorilla
{
    template<typename KEY>
    struct HashMapFunction { static inline uint32_t generate(const KEY& key) { return (uint32_t)key; } };
    template<>
    struct HashMapFunction<char*> { static inline uint32_t generate(const char*& key) { return Hash::generate(key); } };

    template <typename KEY, typename TYPE, typename HASH = HashMapFunction<KEY>>
    class HashMap
    {
    public:
        HashMap();
        ~HashMap();

        inline uint32_t     get_size    () const { return m_nodes.get_size(); }
        inline uint32_t     get_capacity() const { return m_nodes.get_capacity(); }
        inline bool         is_empty    () const { return m_nodes.is_empty(); }
        inline void         clear       () { MEMSET(&m_nodes[0], -1, sizeof(Node) * m_nodes.get_capacity()); m_nodes.clear(); }

        TYPE&               get         (const KEY& key, const TYPE& default_value) const;

        void                add         (const KEY& key, const TYPE& value);
        void                remove      (const KEY& key);

        void                operator=   (const HashMap<KEY, TYPE, HASH>& value) { m_nodes = value.m_nodes; };
        TYPE&               operator[]  (const KEY& key) const;

    private:
        /******************************************************************************
        **  Node
        ******************************************************************************/
        struct Node
        {
            enum class State
            {
                REMOVED = -1,
                UNUSED = 0,
                USED = 1,
            };

            Node()
                : state(State::UNUSED)
            {
                // Nothing to do
            }

            inline bool     is_available() const { return state <= State::UNUSED; }

            TYPE    value;
            KEY     key;
            State   state;
        };

    public:
        /******************************************************************************
        **  Iterator
        ******************************************************************************/
        class Iterator
        {
            friend class HashMap;

        private:
            Iterator(Node *node, Node *last)
                : m_node(node), m_last(last)
         {
             // Nothing to do
         }

        public:
            ~Iterator()
            {
                // Nothing to do
            }

            inline KEY          get_key     () const { return m_node->key; }
            inline TYPE&        get_value   () { return m_node->value; }

            inline bool         operator==  (const Iterator& it) const { return m_node == it.m_node; }
            inline bool         operator!=  (const Iterator& it) const { return m_node != it.m_node; }
            inline Iterator&    operator++  () { while(++m_node != m_last && m_node->state != Node::State::USED) {} return *this; }
            inline Iterator     operator++  (int /*value*/) { Iterator self = *this; ++self; return self; }

        private:
            Node *m_node;
            Node *m_last;
        };

        inline Iterator         get_first();
        inline Iterator         get_last();

    private:
        inline Node*            find_node(const KEY& key) { return const_cast<Node*>(find_node(&m_nodes[0], m_nodes.get_capacity(), key)); }
        inline const Node*      find_node(const KEY& key) const { return find_node(&m_nodes[0], m_nodes.get_capacity(), key); }
        const Node*             find_node(const Node *array, uint32_t capacity, const KEY& key) const;

        void                    reserve(uint32_t capacity);

    private:
        Node            m_stack[HASH_MAP_STACK_CAPACITY];
        Vector<Node>    m_nodes;
    };

    //!	@brief      Constructor
    template <typename KEY, typename TYPE, typename HASH>
    HashMap<KEY, TYPE, HASH>::HashMap()
    {
        m_nodes.set_buffer(m_stack, 0, HASH_MAP_STACK_CAPACITY, false);
    }

    //!	@brief      Destructor
    template <typename KEY, typename TYPE, typename HASH>
    HashMap<KEY, TYPE, HASH>::~HashMap()
    {
        // Nothing to do
    }

    //!	@brief      Add
    template <typename KEY, typename TYPE, typename HASH>
    void HashMap<KEY, TYPE, HASH>::add(const KEY& key, const TYPE& value)
    {
        // If not present so we need to allocate more space
        Node *node = find_node(key);
        while(!node)
        {
            reserve(m_nodes.get_capacity() << HASH_MAP_BLOCK_ALLOCATION);
            node = find_node(key);
        }

        // Apply size only if needed
        if(node->is_available())
        {
            m_nodes.resize(m_nodes.get_size()+1);
        }

        // Update key and value associated
        node->key = key;
        node->value = value;
        node->state = Node::State::USED;
    }

    //!	@brief      Remove
    template <typename KEY, typename TYPE, typename HASH>
    void HashMap<KEY, TYPE, HASH>::remove(const KEY& key)
    {
        Node *node = find_node(key);
        if (node)
        {
            node->State = Node::State::REMOVED;
            m_nodes.resize(m_nodes.get_size()-1);
        }
    }

    //!	@brief      get
    template <typename KEY, typename TYPE, typename HASH>
    TYPE& HashMap<KEY, TYPE, HASH>::get(const KEY& key, const TYPE& default_value) const
    {
        const Node *node = find_node(key);
        if(node && node->state == Node::State::USED)
        {
            return const_cast<TYPE&>(node->value);
        }
        return const_cast<TYPE&>(default_value);
    }

    //!	@brief      GetFirst
    template <typename KEY, typename TYPE, typename HASH>
    typename HashMap<KEY, TYPE, HASH>::Iterator HashMap<KEY, TYPE, HASH>::get_first()
    {
        Iterator it(&m_nodes[0], &m_nodes[m_nodes.get_capacity()]);
        if(it.m_node->state != Node::State::USED)
        {
            ++it;
        }

        return it;
    }

    //!	@brief      GetLast
    template <typename KEY, typename TYPE, typename HASH>
    typename HashMap<KEY, TYPE, HASH>::Iterator HashMap<KEY, TYPE, HASH>::get_last()
    {
        Node *last_node = &m_nodes[m_nodes.get_capacity()];
        return Iterator(last_node, last_node);
    }

    //!	@brief      FindNode
    template <typename KEY, typename TYPE, typename HASH>
    const typename HashMap<KEY, TYPE, HASH>::Node* HashMap<KEY, TYPE, HASH>::find_node(const typename HashMap<KEY, TYPE, HASH>::Node *array, uint32_t capacity, const KEY& key) const
    {
        uint32_t step = 1;
        uint32_t node_index = HASH::generate(key) & (capacity - 1);
        while (node_index < capacity)
        {
            const HashMap<KEY, TYPE, HASH>::Node *node = &array[node_index];
            if (node->is_available() || key == node->key)
            {
                return node;
            }
            node_index += step++;
        }

        return nullptr;
    }

    //!	@brief      Reserve
    template <typename KEY, typename TYPE, typename HASH>
    void HashMap<KEY, TYPE, HASH>::reserve(uint32_t capacity)
    {
        const uint32_t old_capacity = m_nodes.get_capacity();
        if (capacity > old_capacity)
        {
            // Keep reference of old array
            Node *element = &m_nodes[0];
            Node *last_element = &m_nodes[old_capacity];

            // Allocate new array and set proper capacity
            Node *array = new Node[capacity];

            // Iterates old array to reaffect all iterator
            while (element != last_element)
            {
                if (element->state != Node::State::UNUSED)
                {
                    Node *node = const_cast<Node*>(find_node(array, capacity, element->key));
                    *node = *element;
                }
                ++element;
            }

            m_nodes.set_buffer(array, m_nodes.get_size(), capacity, true);
        }
    }

    //!	@brief      operator[]
    template <typename KEY, typename TYPE, typename HASH>
    TYPE& HashMap<KEY, TYPE, HASH>::operator[](const KEY& key) const
    {
        const Node *node = find_node(key);
        if(node && node->state == Node::State::USED)
        {
            return const_cast<TYPE&>(node->value);
        }

        static const TYPE default_value = (TYPE)-1;
        return const_cast<TYPE&>(default_value);
    }
}

#undef HASH_MAP_STACK_CAPACITY
#undef HASH_MAP_BLOCK_ALLOCATION
